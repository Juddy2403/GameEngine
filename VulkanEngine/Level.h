#pragma once
#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

#include "meshes/2DMesh.h"
#include "meshes/3DMesh.h"
#include "meshes/Vertex.h"

class DescriptorPool;
class Level
{
public:
    static int m_AreNormalsEnabled;
    static DescriptorPool& Get2DDescriptorPool() { return m_2DDescriptorPool; }

	void InitializeLevel(const VkCommandPool &commandPool, const glm::mat4 &projMatrix);
	void Update(uint32_t currentFrame, const glm::mat4 &viewMatrix);
	void DestroyLevel() const;
	
	void Draw3DMeshes(const VkCommandBuffer &commandBuffer, uint32_t currentFrame) const;
    void Draw2DMeshes(VkCommandBuffer const &commandBuffer, uint32_t currentFrame) const;
    void WindowHasBeenResized(const glm::mat4& projMatrix);
private:
	std::vector<std::unique_ptr<Mesh3D>> m_3DMeshes;
	std::vector<std::unique_ptr<Mesh2D>> m_2DMeshes;
	UniformBufferObject m_3DUBOMatrixes{};
	UniformBufferObject m_2DUBOMatrixes{};

	static DescriptorPool m_2DDescriptorPool;
};
