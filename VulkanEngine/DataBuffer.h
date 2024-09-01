#pragma once
#include <vulkan/vulkan_core.h>

namespace VulkanEngine
{
    class DataBuffer {
    public:
        DataBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        ~DataBuffer() = default; 
        DataBuffer(const DataBuffer& other) = delete;
        DataBuffer& operator=(const DataBuffer& other) = delete;
        DataBuffer(DataBuffer&& other) noexcept = delete; 
        DataBuffer& operator=(DataBuffer&& other) noexcept = delete;

        void MapAndUpload(VkDeviceSize size, const void* data, VkCommandPool const& commandPool, VkQueue const& graphicsQueue);
        void Destroy() const;

        //Used for the draw function inside Mesh3D
        void BindAsVertexBuffer(VkCommandBuffer commandBuffer) const;
        void BindAsIndexBuffer(VkCommandBuffer commandBuffer) const;
        [[nodiscard]] VkBuffer GetVkBuffer() const { return m_VkBuffer; }
        //[[nodiscard]] VkDeviceSize GetSizeInBytes() const { return m_Size; }

        static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    private:
        static void CopyBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        bool m_HasBeenMapped{false};
        VkBufferUsageFlags m_Usage{};
        VkMemoryPropertyFlags m_Properties{};
        VkBuffer m_VkBuffer{};
        VkBuffer m_StagingBuffer{};
        VkDeviceMemory m_StagingBufferMemory{};
        VkDeviceMemory m_VkBufferMemory{};
        VkDeviceSize m_Size{};

    };
}

