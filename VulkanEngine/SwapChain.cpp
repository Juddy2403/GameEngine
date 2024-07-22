#include "SwapChain.h"
#include "vulkanbase/VulkanBase.h"

SwapChainSupportDetails SwapChain::QuerySwapChainSupport(const VkSurfaceKHR& surface)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanBase::physicalDevice, surface, &details.m_Capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanBase::physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.m_Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanBase::physicalDevice, surface, &formatCount, details.m_Formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanBase::physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.m_PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanBase::physicalDevice, surface, &presentModeCount, details.m_PresentModes.data());
    }

    return details;
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
    if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) return capabilities.currentExtent;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = { static_cast<uint32_t>(width),static_cast<uint32_t>(height) };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return availableFormat;
    }
    return availableFormats[0];
}

void SwapChain::CreateSwapChain(const VkSurfaceKHR& surface, GLFWwindow* window, const QueueFamilyIndices& indices)
{
    const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(surface);

    const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.m_Formats);
    const VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.m_PresentModes);
    const VkExtent2D extent = ChooseSwapExtent(swapChainSupport.m_Capabilities, window);

    uint32_t imageCount = swapChainSupport.m_Capabilities.minImageCount + 1;
    if (swapChainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_Capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.m_Capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (!indices.m_GraphicsFamily.has_value() || !indices.m_PresentFamily.has_value()) throw std::runtime_error("Queue Family Indices are not valid");
    const uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamily.value(),indices.m_PresentFamily.value() };

    if (indices.m_GraphicsFamily != indices.m_PresentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.m_Capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(VulkanBase::device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) throw std::runtime_error("failed to create swap chain!");

    vkGetSwapchainImagesKHR(VulkanBase::device, m_SwapChain, &imageCount, nullptr);
    m_ImageView.m_SwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(VulkanBase::device, m_SwapChain, &imageCount, m_ImageView.m_SwapChainImages.data());

    m_ImageView.m_SwapChainImageFormat = surfaceFormat.format;
    VulkanBase::swapChainExtent = extent;
}

void SwapChain::DestroySwapChain() const
{
    for (const auto& imageView : m_ImageView.m_SwapChainImageViews)
    {
        vkDestroyImageView(VulkanBase::device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(VulkanBase::device, m_SwapChain, nullptr);
}
