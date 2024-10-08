#include "Shader.h"
#include <vulkanbase/VulkanBase.h>

using namespace VulkanEngine;
VkDescriptorSetLayout Shader::m_DescriptorSetLayout;

Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile):
    m_VertexShaderFile{ std::move(vertexShaderFile) },
    m_FragmentShaderFile{ std::move(fragmentShaderFile) } {}

void Shader::Initialize()
{
    m_ShaderStages.push_back(CreateVertexShaderInfo());
    m_ShaderStages.push_back(CreateFragmentShaderInfo());
}

void Shader::DestroyShaderModules()
{
    for (const VkPipelineShaderStageCreateInfo& stageInfo : m_ShaderStages)
    {
        vkDestroyShaderModule(VulkanBase::m_Device, stageInfo.module, nullptr);
    }
    m_ShaderStages.clear();
}

VkPipelineShaderStageCreateInfo Shader::CreateFragmentShaderInfo() const
{
    const std::vector<char> fragShaderCode = ReadFile(m_FragmentShaderFile);
    const VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    return fragShaderStageInfo;
}

VkPipelineShaderStageCreateInfo Shader::CreateVertexShaderInfo() const
{
    const std::vector<char> vertShaderCode = ReadFile(m_VertexShaderFile);
    const VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";
    return vertShaderStageInfo;
}

VkPipelineInputAssemblyStateCreateInfo Shader::CreateInputAssemblyStateInfo()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return inputAssembly;
}

VkShaderModule Shader::CreateShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(VulkanBase::m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) throw std::runtime_error("failed to create shader module!");

    return shaderModule;
}

void Shader::CreateDescriptor()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding;
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutBinding samplerLayoutBinding;
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding normalSamplerLayoutBinding;
    normalSamplerLayoutBinding.binding = 2;
    normalSamplerLayoutBinding.descriptorCount = 1;
    normalSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normalSamplerLayoutBinding.pImmutableSamplers = nullptr;
    normalSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding glossSamplerLayoutBinding;
    glossSamplerLayoutBinding.binding = 3;
    glossSamplerLayoutBinding.descriptorCount = 1;
    glossSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    glossSamplerLayoutBinding.pImmutableSamplers = nullptr;
    glossSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding specularSamplerLayoutBinding;
    specularSamplerLayoutBinding.binding = 4;
    specularSamplerLayoutBinding.descriptorCount = 1;
    specularSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    specularSamplerLayoutBinding.pImmutableSamplers = nullptr;
    specularSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    const std::array bindings = { uboLayoutBinding,samplerLayoutBinding,normalSamplerLayoutBinding,glossSamplerLayoutBinding,specularSamplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(VulkanBase::m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void Shader::DestroyDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(VulkanBase::m_Device, m_DescriptorSetLayout, nullptr);
}
