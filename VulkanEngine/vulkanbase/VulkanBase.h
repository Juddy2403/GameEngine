#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN

#include <glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32

#include <glm/glm.hpp>
#include "VulkanUtil.h"
#include "Shader.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "meshes/3DMesh.h"
#include "Level.h"
#include "RenderPass.h"
#include <GraphicsPipeline.h>
#include "TimeManager.h"
#include "Camera.h"
#include "SwapChain.h"
#include <unordered_set>

namespace VulkanEngine
{
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    enum class ShadingMode
    {
        lambertMode,
        normalMode,
        specularMode,
        combinedMode
    };

    class VulkanBase
    {
    public:
        void Run()
        {
            InitWindow();
            InitVulkan();
            MainLoop();
            Cleanup();
        }

        static VkPhysicalDevice m_PhysicalDevice;
        static VkDevice m_Device;
        static VkExtent2D m_SwapChainExtent;
        static VkQueue m_GraphicsQueue;
    private:
        void InitVulkan()
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

        void MainLoop()
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

        void Cleanup() const
        {
            vkDestroySemaphore(m_Device, m_RenderFinishedSemaphore, nullptr);
            vkDestroySemaphore(m_Device, m_ImageAvailableSemaphore, nullptr);
            vkDestroyFence(m_Device, m_InFlightFence, nullptr);

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

        void CreateSurface()
        {
            if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface) != VK_SUCCESS) throw std::runtime_error("failed to create window surface!");
        }

        ShadingMode m_ShadingMode = ShadingMode::combinedMode;
        static uint32_t m_Width;
        static uint32_t m_Height;
        static bool m_HasWindowResized;
        Camera m_Camera{};
        Shader m_3DShader{
            "shaders/CompiledSpv/3Dshader.vert.spv",
            "shaders/CompiledSpv/3Dshader.frag.spv" };
        Shader m_2DShader{
            "shaders/CompiledSpv/2Dshader.vert.spv",
            "shaders/CompiledSpv/2Dshader.frag.spv" };

        CommandPool m_CommandPool{};
        CommandBuffer m_CommandBuffer{};
        Level m_Level{};
        RenderPass m_RenderPass{};
        GraphicsPipeline m_3DGraphicsPipeline{};
        GraphicsPipeline m_2DGraphicsPipeline{};
        glm::vec2 m_LastMousePos{};
        std::unordered_set<int> m_PressedKeys;
        GLFWwindow* m_Window = nullptr;

        void InitWindow();
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vkDevice) const;
        void DrawFrame(uint32_t imageIndex);
        SwapChain m_SwapChain{};
        VkQueue m_PresentQueue = VK_NULL_HANDLE;
        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        void CreateLogicalDevice();

        // Main initialization
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence m_InFlightFence = VK_NULL_HANDLE;

        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void SetupDebugMessenger();
        static std::vector<const char*> GetRequiredExtensions();
        static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        void CreateInstance();
        void CreateSyncObjects();
        void DrawFrame();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType
            , const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
        {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        }

        void KeyEvent(int key, int scancode, int action, int mods);
        void MouseMove(GLFWwindow* window, double xpos, double ypos);
        void MouseEvent(GLFWwindow* window, int button, int action, int mods);
        void ProcessInput();
        static void WindowResized(GLFWwindow* window, int width, int height);
    };
}
