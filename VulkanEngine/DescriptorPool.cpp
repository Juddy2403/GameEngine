#include "DescriptorPool.h"
#include <vulkanbase/VulkanBase.h>

#include "DataBuffer.h"

using namespace VulkanEngine;
const std::vector<VkDescriptorSet>& DescriptorPool::GetDescriptorSets() const
{
    return m_DescriptorSets;
}

void DescriptorPool::UpdateUniformBuffer(const uint32_t currentFrame, const UniformBufferObject& ubo) const
{
    memcpy(m_UniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}

void DescriptorPool::DestroyUniformBuffers() const
{
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(VulkanBase::m_Device, m_UniformBuffers[i], nullptr);
        vkFreeMemory(VulkanBase::m_Device, m_UniformBuffersMemory[i], nullptr);
    }
    vkDestroyDescriptorPool(VulkanBase::m_Device, m_DescriptorPool, nullptr);
}

void DescriptorPool::CreateUniformBuffers()
{
    m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    m_UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        constexpr VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        DataBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBuffersMemory[i]);
        vkMapMemory(VulkanBase::m_Device, m_UniformBuffersMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
    }
}

void DescriptorPool::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 5> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[3].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[4].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[4].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(VulkanBase::m_Device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) throw std::runtime_error("failed to create descriptor pool!");
}

void DescriptorPool::CreateDescriptorSets()
{
    const std::vector layouts(MAX_FRAMES_IN_FLIGHT, Shader::GetDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(VulkanBase::m_Device, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) throw std::runtime_error("failed to allocate descriptor sets!");

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) UpdateDescriptorSets(i);
}

void DescriptorPool::UpdateDescriptorSets(const uint32_t currentFrame) const
{
    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = m_UniformBuffers[currentFrame];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    if (m_AlbedoImageView == VK_NULL_HANDLE)
    {
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_DescriptorSets[currentFrame];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional
        vkUpdateDescriptorSets(VulkanBase::m_Device, 1, &descriptorWrite, 0, nullptr);
    }
    else
    {
        VkDescriptorImageInfo imageInfo;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_AlbedoImageView;
        imageInfo.sampler = Texture::GetTextureSampler();

        VkDescriptorImageInfo normalImageInfo;
        normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        normalImageInfo.imageView = m_NormalImageView;
        normalImageInfo.sampler = Texture::GetTextureSampler();

        VkDescriptorImageInfo glossImageInfo;
        glossImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        glossImageInfo.imageView = m_GlossImageView;
        glossImageInfo.sampler = Texture::GetTextureSampler();

        VkDescriptorImageInfo specularImageInfo;
        specularImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        specularImageInfo.imageView = m_SpecularImageView;
        specularImageInfo.sampler = Texture::GetTextureSampler();

        std::array<VkWriteDescriptorSet, 5> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = m_DescriptorSets[currentFrame];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = m_DescriptorSets[currentFrame];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = m_DescriptorSets[currentFrame];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pImageInfo = &normalImageInfo;

        descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[3].dstSet = m_DescriptorSets[currentFrame];
        descriptorWrites[3].dstBinding = 3;
        descriptorWrites[3].dstArrayElement = 0;
        descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[3].descriptorCount = 1;
        descriptorWrites[3].pImageInfo = &glossImageInfo;

        descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[4].dstSet = m_DescriptorSets[currentFrame];
        descriptorWrites[4].dstBinding = 4;
        descriptorWrites[4].dstArrayElement = 0;
        descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[4].descriptorCount = 1;
        descriptorWrites[4].pImageInfo = &specularImageInfo;

        vkUpdateDescriptorSets(VulkanBase::m_Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void DescriptorPool::Initialize()
{
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
}

void DescriptorPool::SetAlbedoImageView(const VkImageView imageView)
{
    m_AlbedoImageView = imageView;
}

void DescriptorPool::SetNormalImageView(const VkImageView imageView)
{
    m_NormalImageView = imageView;
}

void DescriptorPool::SetGlossImageView(const VkImageView imageView)
{
    m_GlossImageView = imageView;
}

void DescriptorPool::SetSpecularImageView(const VkImageView imageView)
{
    m_SpecularImageView = imageView;
}
