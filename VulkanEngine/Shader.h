#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include "meshes/Vertex.h"

class Shader
{
public:
    Shader(std::string vertexShaderFile, std::string fragmentShaderFile);
    ~Shader() = default;
    
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(const Shader&&) = delete;
    Shader& operator=(const Shader&&) = delete;
    
    void Initialize();
    void DestroyShaderModules();

    static VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo();
    [[nodiscard]] VkPipelineShaderStageCreateInfo CreateFragmentShaderInfo() const;
    [[nodiscard]] VkPipelineShaderStageCreateInfo CreateVertexShaderInfo() const;

    static void CreateDescriptor();
    static const VkDescriptorSetLayout& GetDescriptorSetLayout() { return m_DescriptorSetLayout; }
    std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() { return m_ShaderStages; }
    static void DestroyDescriptorSetLayout();
private:
    static VkShaderModule CreateShaderModule(const std::vector<char>& code);

    std::string m_VertexShaderFile;
    std::string m_FragmentShaderFile;
    std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
    static VkDescriptorSetLayout m_DescriptorSetLayout;
    UniformBufferObject m_UBOSrc{};
};
