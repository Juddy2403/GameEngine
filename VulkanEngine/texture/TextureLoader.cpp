#include "TextureLoader.h"
#include "DescriptorPool.h"
#include "TextureManager.h"

using namespace VulkanEngine;
Texture TextureLoader::m_DefaultTexture;
TextureLoader::TextureLoader(DescriptorPool& descriptorPool): m_DescriptorPool(descriptorPool) {
    m_DescriptorPool.SetAlbedoImageView(m_DefaultTexture.GetTextureImageView());
    m_DescriptorPool.SetNormalImageView(m_DefaultTexture.GetTextureImageView());
    m_DescriptorPool.SetGlossImageView(m_DefaultTexture.GetTextureImageView());
    m_DescriptorPool.SetSpecularImageView(m_DefaultTexture.GetTextureImageView());
}
void TextureLoader::LoadDefaultTexture(VkCommandPool const &commandPool, const std::string &path) {
    m_DefaultTexture = TextureManager::GetInstance().LoadTexture(commandPool,path);
}

void TextureLoader::UploadAlbedoTexture(const VkCommandPool &commandPool, const std::string &path) {
    m_AlbedoTexture = TextureManager::GetInstance().LoadTexture(commandPool,path);
    m_DescriptorPool.DestroyUniformBuffers();
    m_DescriptorPool.SetAlbedoImageView(m_AlbedoTexture.GetTextureImageView());
    m_DescriptorPool.Initialize();

}

void TextureLoader::UploadNormalTexture(VkCommandPool const &commandPool, const std::string &path) {
    m_NormalTexture = TextureManager::GetInstance().LoadTexture(commandPool,path);
    m_DescriptorPool.DestroyUniformBuffers();
    m_DescriptorPool.SetNormalImageView(m_NormalTexture.GetTextureImageView());
    m_DescriptorPool.Initialize();
    m_HasNormalMap = 1;
}

void TextureLoader::UploadGlossTexture(VkCommandPool const &commandPool, const std::string &path) {
    m_GlossTexture = TextureManager::GetInstance().LoadTexture(commandPool,path);
    m_DescriptorPool.DestroyUniformBuffers();
    m_DescriptorPool.SetGlossImageView(m_GlossTexture.GetTextureImageView());
    m_DescriptorPool.Initialize();
}

void TextureLoader::UploadSpecularTexture(VkCommandPool const &commandPool, const std::string &path) {
    m_SpecularTexture = TextureManager::GetInstance().LoadTexture(commandPool,path);
    m_DescriptorPool.DestroyUniformBuffers();
    m_DescriptorPool.SetSpecularImageView(m_SpecularTexture.GetTextureImageView());
    m_DescriptorPool.Initialize();
}
