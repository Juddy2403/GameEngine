#include "DepthBuffer.h"
#include <vulkanbase/VulkanBase.h>

using namespace VulkanEngine;
void DepthBuffer::CreateDepthResources()
{
    const VkFormat depthFormat = FindDepthFormat();

    ImageInfoStruct imageInfo;
    imageInfo.width = VulkanBase::swapChainExtent.width;
    imageInfo.height = VulkanBase::swapChainExtent.height;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    Texture::CreateImage(imageInfo, m_DepthImage, m_DepthImageMemory);

    m_DepthImageView = ImageView::CreateImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat DepthBuffer::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features)
{
    for (const VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(VulkanBase::physicalDevice, format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) return format;
        if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) return format;
    }
    throw std::runtime_error("failed to find supported format!");
}

VkFormat DepthBuffer::FindDepthFormat()
{
    return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT,VK_FORMAT_D32_SFLOAT_S8_UINT,VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool DepthBuffer::HasStencilComponent(const VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void DepthBuffer::DestroyDepthResources() const
{
    vkDestroyImageView(VulkanBase::device, m_DepthImageView, nullptr);
    vkDestroyImage(VulkanBase::device, m_DepthImage, nullptr);
    vkFreeMemory(VulkanBase::device, m_DepthImageMemory, nullptr);
}
