#pragma once
#include "Texture.h"

class DescriptorPool;
class TextureLoader
{
public:
    static Texture m_DefaultTexture;
    int m_HasNormalMap = 0;

    explicit TextureLoader(DescriptorPool& descriptorPool);
    static void LoadDefaultTexture(VkCommandPool const &commandPool, const std::string &path);
    void UploadAlbedoTexture(VkCommandPool const &commandPool, const std::string &path);
    void UploadNormalTexture(VkCommandPool const &commandPool, const std::string &path);
    void UploadGlossTexture(VkCommandPool const &commandPool, const std::string &path);
    void UploadSpecularTexture(VkCommandPool const &commandPool, const std::string &path);

private:
    Texture m_AlbedoTexture{};
    Texture m_NormalTexture{};
    Texture m_GlossTexture{};
    Texture m_SpecularTexture{};
//TODO: switch to a pointer instead of ref so you can check if it's nullptr
    DescriptorPool& m_DescriptorPool;
};


