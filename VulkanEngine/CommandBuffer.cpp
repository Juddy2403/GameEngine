#include "CommandBuffer.h"
#include "vulkanbase/VulkanBase.h"

using namespace VulkanEngine;
CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
{
    m_CommandBuffer = other.m_CommandBuffer;
    other.m_CommandBuffer = VK_NULL_HANDLE;
}
CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
{
    if (this == &other) return *this;
    //TODO: make the command pool accessible and uncomment
    //if (m_CommandBuffer != VK_NULL_HANDLE) FreeCommandBuffer(VulkanBase::m_CommandPool);
    m_CommandBuffer = other.m_CommandBuffer;
    other.m_CommandBuffer = VK_NULL_HANDLE;
    return *this;
}

CommandBuffer::CommandBuffer(const VkCommandPool& commandPool)
{
    m_CommandBuffer = CreateCommandBuffer(commandPool);
}

const VkCommandBuffer& CommandBuffer::GetVkCommandBuffer() const
{
    return m_CommandBuffer;
}
void CommandBuffer::SetVkCommandBuffer(const VkCommandBuffer buffer)
{
    if (buffer == VK_NULL_HANDLE) throw std::runtime_error("CommandBuffer::SetVkCommandBuffer: buffer is VK_NULL_HANDLE");

    m_CommandBuffer = buffer;
}

void CommandBuffer::Reset() const
{
    vkResetCommandBuffer(m_CommandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
}

void CommandBuffer::BeginRecording(const VkCommandBufferUsageFlags& flags) const
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS) throw std::runtime_error("failed to begin recording command buffer!");
}

void CommandBuffer::EndRecording() const
{
    if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) throw std::runtime_error("failed to record command buffer!");
}

void CommandBuffer::Submit(VkSubmitInfo submitInfo, const VkFence fence) const
{
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffer;
    if (vkQueueSubmit(VulkanBase::graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS) throw std::runtime_error("failed to submit draw command buffer!");
    vkQueueWaitIdle(VulkanBase::graphicsQueue);
}

VkCommandBuffer CommandBuffer::CreateCommandBuffer(const VkCommandPool& commandPool)
{
    VkCommandBuffer commandBuffer{};

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(VulkanBase::device, &allocInfo, &commandBuffer) != VK_SUCCESS) throw std::runtime_error("failed to allocate command buffers!");
    
    return commandBuffer;
}

void CommandBuffer::FreeCommandBuffer(const VkCommandPool& commandPool) const
{
    vkFreeCommandBuffers(VulkanBase::device, commandPool, 1, &m_CommandBuffer);
}
