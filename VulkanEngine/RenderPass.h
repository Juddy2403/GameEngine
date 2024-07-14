#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include "DepthBuffer.h"

class RenderPass
{
public:
    RenderPass() = default;
    ~RenderPass() = default;
    RenderPass(const RenderPass& other) = delete;
    RenderPass& operator=(const RenderPass& other) = delete;
    RenderPass(RenderPass&& other) noexcept = delete;
    RenderPass& operator=(RenderPass&& other) noexcept = delete;
    
    static DepthBuffer& GetDepthBuffer() { return m_DepthBuffer; }
    VkRenderPass& GetRenderPass() { return m_RenderPass; }
    std::vector<VkFramebuffer>& GetSwapChainFramebuffers() { return m_SwapChainFramebuffers; }

    void CreateFrameBuffers(const std::vector<VkImageView>& swapChainImageViews, const VkExtent2D& swapChainExtent);
    void CreateRenderPass(const VkFormat& swapChainImageFormat);
    void DestroyRenderPass() const;
private:
    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    static DepthBuffer m_DepthBuffer;
    std::vector<VkFramebuffer> m_SwapChainFramebuffers;
};
