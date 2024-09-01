#include "vulkanbase/VulkanBase.h"
#include <algorithm>
#include <set>

using namespace VulkanEngine;
VkPhysicalDevice VulkanBase::m_PhysicalDevice = VK_NULL_HANDLE;
VkDevice VulkanBase::m_Device = VK_NULL_HANDLE;
VkExtent2D VulkanBase::m_SwapChainExtent;
uint32_t VulkanBase::m_Width = 800;
uint32_t VulkanBase::m_Height = 600;
VkQueue VulkanBase::m_GraphicsQueue;
bool VulkanBase::m_HasWindowResized = false;

void VulkanBase::WindowResized(GLFWwindow* window, const int width, const int height)
{
    m_Width = width;
    m_Height = height;
    m_HasWindowResized = true;
}

void VulkanBase::InitVulkan()
{
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();

    PickPhysicalDevice();
    CreateLogicalDevice();

    m_SwapChain.CreateSwapChain(m_Surface, m_Window, FindQueueFamilies(m_PhysicalDevice));
    m_SwapChain.GetImageView().CreateImageViews();

    m_3DShader.Initialize();
    m_2DShader.Initialize();
    m_RenderPass.CreateRenderPass(m_SwapChain.GetImageView().m_SwapChainImageFormat);
    Shader::CreateDescriptor();
    GraphicsPipeline::CreatePipelineLayout();

    m_3DGraphicsPipeline.CreateGraphicsPipeline(m_RenderPass.GetRenderPass(), m_3DShader, Vertex3D::CreateVertexInputStateInfo());
    m_2DGraphicsPipeline.CreateGraphicsPipeline(m_RenderPass.GetRenderPass(), m_2DShader, Vertex2D::CreateVertexInputStateInfo(), false);
    m_RenderPass.CreateFrameBuffers(m_SwapChain.GetImageView().m_SwapChainImageViews, m_SwapChainExtent);
    m_Camera.Initialize(45.f, { 0.f,0.f,-2.f }, static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height));

    m_CommandPool = CommandPool{ m_Surface,FindQueueFamilies(m_PhysicalDevice) };
    m_CommandBuffer = CommandBuffer{ m_CommandPool.GetCommandPool() };
    m_Level.InitializeLevel(m_CommandPool.GetCommandPool(), m_Camera.m_ProjectionMatrix);

    CreateSyncObjects();
}
void VulkanBase::MainLoop()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();
        ProcessInput();
        TimeManager::GetInstance().Update();
        if (m_HasWindowResized)
        {
            vkDeviceWaitIdle(m_Device);
            m_SwapChain.DestroySwapChain();
            m_SwapChain.CreateSwapChain(m_Surface, m_Window, FindQueueFamilies(m_PhysicalDevice));
            m_SwapChain.GetImageView().CreateImageViews();
            m_RenderPass.DestroyRenderPass();
            m_RenderPass.CreateRenderPass(m_SwapChain.GetImageView().m_SwapChainImageFormat);
            m_Camera.CalculateProjectionMatrix(static_cast<float>(m_Width) / static_cast<float>(m_Height));
            m_Level.WindowHasBeenResized(m_Camera.m_ProjectionMatrix);
            m_RenderPass.CreateFrameBuffers(m_SwapChain.GetImageView().m_SwapChainImageViews, m_SwapChainExtent);
            m_HasWindowResized = false;
        }
        m_Camera.Update();
        DrawFrame();
    }
    vkDeviceWaitIdle(m_Device);
}
void VulkanBase::Cleanup() const
{
    for (size_t i = 0; i < m_SwapChain.GetImageView().m_SwapChainImages.size(); i++)
    {
        vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
    }

    m_CommandPool.DestroyCommandPool();

    m_3DGraphicsPipeline.DestroyGraphicsPipeline();
    m_2DGraphicsPipeline.DestroyGraphicsPipeline();
    GraphicsPipeline::DestroyGraphicsPipelineLayout();
    Shader::DestroyDescriptorSetLayout();
    m_RenderPass.DestroyRenderPass();

    if (enableValidationLayers) DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    m_SwapChain.DestroySwapChain();

    m_Level.DestroyLevel();
    vkDestroyDevice(m_Device, nullptr);

    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    vkDestroyInstance(m_Instance, nullptr);

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
void VulkanBase::CreateSurface()
{
    if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface) != VK_SUCCESS) throw std::runtime_error("failed to create window surface!");
}
void VulkanBase::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_Window = glfwCreateWindow(m_Width, m_Height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetKeyCallback(m_Window, [](GLFWwindow* glfWwindow, const int key, const int scancode, const int action, const int mods) {
        void* pUser = glfwGetWindowUserPointer(glfWwindow);
        VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
        vBase->KeyEvent(key, scancode, action, mods);
    });
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* glfWwindow, const double xpos, const double ypos) {
        void* pUser = glfwGetWindowUserPointer(glfWwindow);
        VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
        vBase->MouseMove(glfWwindow, xpos, ypos);
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* glfWwindow, const int button, const int action, const int mods) {
        void* pUser = glfwGetWindowUserPointer(glfWwindow);
        VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
        vBase->MouseEvent(glfWwindow, button, action, mods);
    });

    glfwSetFramebufferSizeCallback(m_Window, VulkanBase::WindowResized);
}

void VulkanBase::KeyEvent(const int key, int scancode, const int action, int mods)
{
    if (action == GLFW_PRESS) m_PressedKeys.insert(key);
    else if (action == GLFW_RELEASE) m_PressedKeys.erase(key);

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        Level::m_AreNormalsEnabled = Level::m_AreNormalsEnabled == 1 ? 0 : 1;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        m_ShadingMode = static_cast<ShadingMode>((static_cast<int>(m_ShadingMode) + 1) % 4);
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        m_ShadingMode = static_cast<ShadingMode>((static_cast<int>(m_ShadingMode) - 1));
        if (static_cast<int>(m_ShadingMode) < 0)
            m_ShadingMode = static_cast<ShadingMode>(3);
    }
}

void VulkanBase::ProcessInput()
{
    if (m_PressedKeys.contains(GLFW_KEY_LEFT_SHIFT)) m_Camera.m_MovementSpeed = 15.f;
    else m_Camera.m_MovementSpeed = 10.f;

    if (m_PressedKeys.contains(GLFW_KEY_W))
        m_Camera.m_Origin += (m_Camera.m_MovementSpeed * TimeManager::GetInstance().GetElapsed()) * m_Camera.m_Forward;

    if (m_PressedKeys.contains(GLFW_KEY_S))
        m_Camera.m_Origin -= (m_Camera.m_MovementSpeed * TimeManager::GetInstance().GetElapsed()) * m_Camera.m_Forward;

    if (m_PressedKeys.contains(GLFW_KEY_A))
        m_Camera.m_Origin -= (m_Camera.m_MovementSpeed * TimeManager::GetInstance().GetElapsed()) * m_Camera.m_Right;

    if (m_PressedKeys.contains(GLFW_KEY_D))
        m_Camera.m_Origin += (m_Camera.m_MovementSpeed * TimeManager::GetInstance().GetElapsed()) * m_Camera.m_Right;
}

void VulkanBase::MouseMove(GLFWwindow* window, double xpos, double ypos)
{
    const int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (state == GLFW_PRESS)
    {
        const float dx = m_LastMousePos.x - static_cast<float>(xpos);
        const float dy = m_LastMousePos.y - static_cast<float>(ypos);
        std::cout << dx << " " << dy << std::endl;
        if (dx != 0.f) m_Camera.m_TotalYaw -= m_Camera.m_RotationSpeed * TimeManager::GetInstance().GetElapsed() * dx;
        if (dy != 0.f)
        {
            auto totalPitch = m_Camera.m_TotalPitch - m_Camera.m_RotationSpeed * TimeManager::GetInstance().GetElapsed() * dy;
            if (totalPitch > -60.f && totalPitch < 60.f) m_Camera.m_TotalPitch = totalPitch;
            m_Camera.m_TotalPitch = std::clamp(m_Camera.m_TotalPitch, -60.f, 60.f);
            //std::cout<<m_Camera.m_TotalPitch<<std::endl;
            // m_Camera.m_TotalPitch -= m_Camera.m_RotationSpeed * TimeManager::GetInstance().GetElapsed() * dy;
        }
        if (m_Camera.m_TotalYaw > 360.f) m_Camera.m_TotalYaw -= 360.f;
        if (m_Camera.m_TotalYaw < -360.f) m_Camera.m_TotalYaw += 360.f;
        // m_Camera.m_TotalPitch = std::clamp(m_Camera.m_TotalPitch, -60.f, 60.f);
    }
    m_LastMousePos = { xpos,ypos };
}

void VulkanBase::MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
////        double xpos, ypos;
////        glfwGetCursorPos(window, &xpos, &ypos);
//
//    }
}

void VulkanBase::DrawFrame(const uint32_t imageIndex)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_RenderPass.GetRenderPass();
    renderPassInfo.framebuffer = m_RenderPass.GetSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = { 0,0 };
    renderPassInfo.renderArea.extent = m_SwapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { { 0.2f,0.4f,0.6f,1.0f } };
    clearValues[1].depthStencil = { 1.0f,0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_CommandBuffer.GetVkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_SwapChainExtent.width);
    viewport.height = static_cast<float>(m_SwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_CommandBuffer.GetVkCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset = { 0,0 };
    scissor.extent = m_SwapChainExtent;
    vkCmdSetScissor(m_CommandBuffer.GetVkCommandBuffer(), 0, 1, &scissor);

    vkCmdBindPipeline(m_CommandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_2DGraphicsPipeline.GetGraphicsPipeline());

    m_Level.Draw2DMeshes(m_CommandBuffer.GetVkCommandBuffer(), imageIndex);

    vkCmdBindPipeline(m_CommandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_3DGraphicsPipeline.GetGraphicsPipeline());

    m_Level.Draw3DMeshes(m_CommandBuffer.GetVkCommandBuffer(), imageIndex);

    vkCmdEndRenderPass(m_CommandBuffer.GetVkCommandBuffer());
}

QueueFamilyIndices VulkanBase::FindQueueFamilies(const VkPhysicalDevice vkDevice) const
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.m_GraphicsFamily = i;
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkDevice, i, m_Surface, &presentSupport);
        if (presentSupport) indices.m_PresentFamily = i;
        if (indices.IsComplete()) break;
        i++;
    }
    return indices;
}

void VulkanBase::PickPhysicalDevice() const
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

    if (deviceCount == 0) throw std::runtime_error("failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices{ deviceCount };
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    if (deviceCount == 0) throw std::runtime_error("failed to find GPUs with Vulkan support!");

    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device))
        {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) throw std::runtime_error("failed to find a suitable GPU!");
}

bool VulkanBase::IsDeviceSuitable(const VkPhysicalDevice device) const
{
    const QueueFamilyIndices indices = FindQueueFamilies(device);
    const bool extensionsSupported = CheckDeviceExtensionSupport(device);

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.IsComplete() && extensionsSupported && supportedFeatures.samplerAnisotropy;
}

void VulkanBase::CreateLogicalDevice()
{
    auto [graphicsFamily, presentFamily] = FindQueueFamilies(m_PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily.value(),presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) throw std::runtime_error("failed to create logical device!");

    vkGetDeviceQueue(m_Device, graphicsFamily.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, presentFamily.value(), 0, &m_PresentQueue);
}

void VulkanBase::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
}

void VulkanBase::SetupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) throw std::runtime_error("failed to set up debug messenger!");
}

void VulkanBase::CreateSyncObjects()
{
    m_ImageAvailableSemaphores.resize(m_SwapChain.GetImageView().m_SwapChainImages.size());
    m_RenderFinishedSemaphores.resize(m_SwapChain.GetImageView().m_SwapChainImages.size());
    m_InFlightFences.resize(m_SwapChain.GetImageView().m_SwapChainImages.size());

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < m_SwapChain.GetImageView().m_SwapChainImages.size(); i++)
    {
        if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_Device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    // VkSemaphoreCreateInfo semaphoreInfo{};
    // semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    //
    // VkFenceCreateInfo fenceInfo{};
    // fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    // fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    //
    // if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore) != VK_SUCCESS ||
    //     vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore) != VK_SUCCESS ||
    //     vkCreateFence(m_Device, &fenceInfo, nullptr, &m_InFlightFence) != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to create synchronization objects for a frame!");
    // }
}

void VulkanBase::DrawFrame()
{
    vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]);

    static uint32_t imageIndex;
    if(vkAcquireNextImageKHR(m_Device, m_SwapChain.GetSwapChain(), UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS)
        throw std::runtime_error("failed to acquire swap chain image!");
    // Ensure imageIndex is within the bounds of MAX_FRAMES_IN_FLIGHT
    imageIndex = imageIndex % MAX_FRAMES_IN_FLIGHT;

    m_CommandBuffer.Reset();
    m_CommandBuffer.BeginRecording();
    vkCmdPushConstants(m_CommandBuffer.GetVkCommandBuffer(), GraphicsPipeline::GetPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &m_Camera.m_Origin);
    vkCmdPushConstants(m_CommandBuffer.GetVkCommandBuffer(), GraphicsPipeline::GetPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::vec3) + sizeof(int), sizeof(int), &m_ShadingMode);
    m_Level.Update(imageIndex, m_Camera.m_ViewMatrix);

    DrawFrame(imageIndex);
    m_CommandBuffer.EndRecording();

    VkSubmitInfo submitInfo{};
    const VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
    constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    const VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    m_CommandBuffer.Submit(submitInfo, m_InFlightFences[m_CurrentFrame]);

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    const VkSwapchainKHR swapChains[] = { m_SwapChain.GetSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    
    vkQueuePresentKHR(m_PresentQueue, &presentInfo);
    m_CurrentFrame = (m_CurrentFrame + 1) % m_SwapChain.GetImageView().m_SwapChainImages.size();
}

bool CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) return false;
    }

    return true;
}

std::vector<const char*> VulkanBase::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extensions;
}

bool VulkanBase::CheckDeviceExtensionSupport(const VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

void VulkanBase::CreateInstance()
{
    if (enableValidationLayers && !CheckValidationLayerSupport()) throw std::runtime_error("validation layers requested, but not available!");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto extensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) throw std::runtime_error("failed to create instance!");
}
