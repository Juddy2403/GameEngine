#pragma once
#include <vulkan/vulkan_core.h>

class Shader;
class GraphicsPipeline
{
public:
    GraphicsPipeline() = default;
    ~GraphicsPipeline() = default;
    GraphicsPipeline(const GraphicsPipeline& other) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
    GraphicsPipeline(GraphicsPipeline&& other) noexcept = delete;
    GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept = delete;

    VkPipeline& GetGraphicsPipeline() { return m_GraphicsPipeline; }
    void CreateGraphicsPipeline(const VkRenderPass& renderPass, Shader& gradientShader, VkPipelineVertexInputStateCreateInfo pipelineVerInputStateCreateInfo, bool enableDepthBuffering = true);
    void DestroyGraphicsPipeline() const;

    static VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }
    static void DestroyGraphicsPipelineLayout();
    static void CreatePipelineLayout();
private:
    VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;
    static VkPipelineLayout m_PipelineLayout;
};
