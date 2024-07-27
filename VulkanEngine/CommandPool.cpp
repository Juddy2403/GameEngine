#include "CommandPool.h"
#include "vulkanbase/VulkanBase.h"

using namespace VulkanEngine;
VkCommandPool CommandPool::CreateCommandPool(const VkSurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices,
    const VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
{
    VkCommandPool commandPool{};
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = flags;
    if (queueFamilyIndices.m_GraphicsFamily.has_value()) poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();
    else throw std::runtime_error("No queue family found!");
    if (vkCreateCommandPool(VulkanBase::m_Device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) throw std::runtime_error("failed to create command pool!");
    return commandPool;
}

CommandPool::CommandPool(CommandPool&& other) noexcept
{
    m_CommandPool = other.m_CommandPool;
    other.m_CommandPool = VK_NULL_HANDLE;
}
CommandPool& CommandPool::operator=(CommandPool&& other) noexcept
{
    if (this == &other) return *this;
    if (m_CommandPool != VK_NULL_HANDLE) DestroyCommandPool();
    m_CommandPool = other.m_CommandPool;
    other.m_CommandPool = VK_NULL_HANDLE;
    return *this;
}

void CommandPool::DestroyCommandPool() const
{
    vkDestroyCommandPool(VulkanBase::m_Device, m_CommandPool, nullptr);
}

CommandPool::CommandPool(const VkSurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices)
{
    m_CommandPool = CreateCommandPool(surface, queueFamilyIndices);
}

const VkCommandPool& CommandPool::GetCommandPool() const
{
    return m_CommandPool;
}
