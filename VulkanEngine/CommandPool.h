#pragma once
#include <vulkan/vulkan_core.h>
#include <optional>

namespace VulkanEngine
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> m_GraphicsFamily;
        std::optional<uint32_t> m_PresentFamily;

        [[nodiscard]] bool IsComplete() const
        {
            return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
        }
    };

    class CommandPool
    {
    public:
        CommandPool() = default;
        ~CommandPool() = default;
        CommandPool(const CommandPool& other) = delete;
        CommandPool& operator=(const CommandPool& other) = delete;
        CommandPool(CommandPool&& other) noexcept;
        CommandPool& operator=(CommandPool&& other) noexcept;

        void DestroyCommandPool() const;
        explicit CommandPool(const VkSurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices);
        const VkCommandPool& GetCommandPool() const;
    private:
        static VkCommandPool CreateCommandPool(const VkSurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices, VkCommandPoolCreateFlags flags);
        VkCommandPool m_CommandPool{};
    };
}
