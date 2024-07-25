#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <array>

//to fix the alignment requirements most of the time
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace VulkanEngine
{
    struct Vertex3D
    {
        glm::vec3 m_Pos{};
        glm::vec3 m_Normal{};
        glm::vec3 m_Color{ 1,1,1 };
        glm::vec2 m_TexCoord{};
        glm::vec3 m_Tangent{};

        static VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo()
        {
            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

            static auto bindingDescription = Vertex3D::GetBindingDescription();
            static auto attributeDescriptions = Vertex3D::GetAttributeDescriptions();

            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            return vertexInputInfo;
        }

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription;
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex3D);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 5> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex3D, m_Pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 2;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex3D, m_Normal);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 4;
            attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex3D, m_Color);

            attributeDescriptions[3].binding = 0;
            attributeDescriptions[3].location = 6;
            attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[3].offset = offsetof(Vertex3D, m_TexCoord);

            attributeDescriptions[4].binding = 0;
            attributeDescriptions[4].location = 8;
            attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[4].offset = offsetof(Vertex3D, m_Tangent);

            return attributeDescriptions;
        }
        bool operator==(const Vertex3D& other) const
        {
            return m_Pos == other.m_Pos && m_Normal == other.m_Normal && m_Color == other.m_Color && m_TexCoord == other.m_TexCoord && m_Tangent == other.m_Tangent;
        }
    };

    struct Vertex2D
    {
        alignas(16) glm::vec2 m_Pos{};
        alignas(16) glm::vec3 m_Color{ 1.f,1.f,1.f };

        static VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo()
        {
            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

            static auto bindingDescription = Vertex2D::GetBindingDescription();
            static auto attributeDescriptions = Vertex2D::GetAttributeDescriptions();

            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            return vertexInputInfo;
        }

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription;
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex2D);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex2D, m_Pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 2;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex2D, m_Color);

            return attributeDescriptions;
        }
        bool operator==(const Vertex2D& other) const
        {
            return m_Pos == other.m_Pos;
        }
    };

    struct UniformBufferObject
    {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };
}

template<> struct std::hash<VulkanEngine::Vertex3D>
{
    size_t operator()(VulkanEngine::Vertex3D const& vertex) const noexcept
    {
        return ((hash<glm::vec3>()(vertex.m_Pos) ^
            (hash<glm::vec3>()(vertex.m_Normal) << 1)) >> 1) ^
        (hash<glm::vec3>()(vertex.m_Color) << 1) ^
        (hash<glm::vec2>()(vertex.m_TexCoord) << 1)
        ^ (hash<glm::vec3>()(vertex.m_Tangent) << 1);
    }
};

template<> struct std::hash<VulkanEngine::Vertex2D>
{
    size_t operator()(VulkanEngine::Vertex2D const& vertex) const noexcept
    {
        return ((hash<glm::vec2>()(vertex.m_Pos) ^
            (hash<glm::vec3>()(vertex.m_Color) << 1)) >> 1);
    }
};
