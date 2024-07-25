#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>

namespace VulkanEngine
{
    class DepthBuffer
    {
    public:
        DepthBuffer() = default;
        ~DepthBuffer() = default;
        DepthBuffer(const DepthBuffer& other) = delete;
        DepthBuffer& operator=(const DepthBuffer& other) = delete;
        DepthBuffer(DepthBuffer&& other) noexcept = delete;
        DepthBuffer& operator=(DepthBuffer&& other) noexcept = delete;

        [[nodiscard]] VkImage GetDepthImage() const { return m_DepthImage; }
        [[nodiscard]] VkImageView GetDepthImageView() const { return m_DepthImageView; }
        static VkFormat FindDepthFormat();
        void CreateDepthResources();
        void DestroyDepthResources() const;
    private:
        VkImage m_DepthImage;
        VkDeviceMemory m_DepthImageMemory;
        VkImageView m_DepthImageView;

        static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        static bool HasStencilComponent(VkFormat format);
    };
}
