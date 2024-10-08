#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

#include "DescriptorPool.h"
#include "Vertex.h"
#include "TrsTransform.h"
#include "texture/TextureLoader.h"

namespace VulkanEngine
{
    struct UniformBufferObject;
    struct Vertex3D;
    class DataBuffer;
    class Mesh3D
    {
    public:
        Mesh3D();
        explicit Mesh3D(std::vector<Vertex3D>&& vertices, std::vector<uint32_t>&& indices);
        Mesh3D(const Mesh3D& other) = delete;
        Mesh3D(Mesh3D&& other) = delete;
        Mesh3D& operator=(const Mesh3D& other) = delete;
        Mesh3D& operator=(Mesh3D&& other) = delete;
        ~Mesh3D() = default;

        TrsTransform& GetTransform() { return m_Transform; }
        TextureLoader& GetTextureLoader() { return m_TextureManager; }
        void SetPBRMaterial() { m_DoesHavePBRMaterial = 1; }
        void AddVertex(const Vertex3D& vertex);
        void Update(uint32_t currentFrame, UniformBufferObject ubo);
        void ClearVertices();
        void ClearIndices();
        void MapAndUploadMesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue) const;

        void Destroy() const;
        void Draw(const VkCommandBuffer& commandBuffer, uint32_t currentFrame) const;
    private:
        int m_DoesHavePBRMaterial = 0;
        std::unique_ptr<DataBuffer> m_VertexBuffer{};
        std::unique_ptr<DataBuffer> m_IndexBuffer{};
        std::unordered_map<Vertex3D, uint32_t> m_UniqueVertices{};
        std::vector<Vertex3D> m_Vertices = {};
        std::vector<uint32_t> m_Indices = {};
        TrsTransform m_Transform;
        TextureLoader m_TextureManager;

        DescriptorPool m_DescriptorPool;
        void DestroyBuffers() const;
    };
}
