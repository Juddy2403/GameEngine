#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include "glfw3.h"
#include "CommandPool.h"
#include "texture/ImageView.h"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR m_Capabilities;
    std::vector<VkSurfaceFormatKHR> m_Formats;
    std::vector<VkPresentModeKHR> m_PresentModes;
};

class SwapChain final {
public:
    SwapChain() = default;
    ~SwapChain() = default;
    SwapChain(const SwapChain &other) = delete;
    SwapChain &operator=(const SwapChain &other) = delete;
    SwapChain(SwapChain &&other) noexcept = delete;
    SwapChain &operator=(SwapChain &&other) noexcept = delete;
    
    static SwapChainSupportDetails QuerySwapChainSupport(const VkSurfaceKHR &surface);
    void CreateSwapChain(const VkSurfaceKHR &surface, GLFWwindow *window, const QueueFamilyIndices &indices);
    VkSwapchainKHR &GetSwapChain() { return m_SwapChain; }
    ImageView &GetImageView() { return m_ImageView; }
    void DestroySwapChain() const;
private:
    VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
    ImageView m_ImageView = ImageView();

    static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);
    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
};


