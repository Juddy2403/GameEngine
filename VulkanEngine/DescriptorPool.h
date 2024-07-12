#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include "meshes/Vertex.h"
//to fix the alignment requirements most of the time
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

class DescriptorPool final
{
public:
    DescriptorPool() = default;
    ~DescriptorPool() = default;
    DescriptorPool(const DescriptorPool& other) = delete;
    DescriptorPool& operator=(const DescriptorPool& other) = delete;
    DescriptorPool(DescriptorPool&& other) noexcept = delete;
    DescriptorPool& operator=(DescriptorPool&& other) noexcept = delete;
    
    void Initialize();
    void DestroyUniformBuffers() const;

    void SetAlbedoImageView(VkImageView imageView);
    void SetNormalImageView(VkImageView imageView);
    void SetGlossImageView(VkImageView imageView);
    void SetSpecularImageView(VkImageView imageView);
    
    [[nodiscard]] const std::vector<VkDescriptorSet>& GetDescriptorSets() const;
    void UpdateUniformBuffer(uint32_t currentFrame, const UniformBufferObject& ubo) const;
    void UpdateDescriptorSets(uint32_t currentFrame) const;
private:
    std::vector<VkBuffer> m_UniformBuffers;
    std::vector<VkDeviceMemory> m_UniformBuffersMemory;
    std::vector<void*> m_UniformBuffersMapped;
    VkDescriptorPool m_DescriptorPool;

    std::vector<VkDescriptorSet> m_DescriptorSets;
    VkImageView m_AlbedoImageView;
    VkImageView m_NormalImageView;
    VkImageView m_GlossImageView;
    VkImageView m_SpecularImageView;

    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
};
