#pragma once
#include <vulkan/vulkan_core.h>

class CommandBuffer
{
public:
    CommandBuffer() = default;
    ~CommandBuffer() = default; 
    CommandBuffer(const CommandBuffer& other) = delete;
    CommandBuffer& operator=(const CommandBuffer& other) = delete;
    CommandBuffer(CommandBuffer&& other) noexcept; 
    CommandBuffer& operator=(CommandBuffer&& other) noexcept; 
    
    explicit CommandBuffer(const VkCommandPool& commandPool);
    [[nodiscard]] const VkCommandBuffer& GetVkCommandBuffer() const;
    void SetVkCommandBuffer(VkCommandBuffer buffer);
    static VkCommandBuffer CreateCommandBuffer(const VkCommandPool& commandPool);
    void FreeCommandBuffer(const VkCommandPool& commandPool) const;

    void Reset() const;
    void BeginRecording(const VkCommandBufferUsageFlags& flags = 0) const;
    void EndRecording() const;
    void Submit(VkSubmitInfo submitInfo = {}, VkFence fence = VK_NULL_HANDLE) const;
private:
    VkCommandBuffer m_CommandBuffer{};
};
