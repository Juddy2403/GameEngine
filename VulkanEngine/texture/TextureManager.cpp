#include "TextureManager.h"
#include <ranges>

Texture TextureManager::LoadTexture(VkCommandPool const& commandPool, const std::string& path)
{
    if (m_TextureMap.contains(path))return m_TextureMap[path];

    Texture texture{};
    texture.CreateTextureImage(commandPool, path);
    m_TextureMap[path] = texture;
    return texture;
}

void TextureManager::DestroyTextures()
{
    for (auto& val : m_TextureMap | std::views::values) val.DestroyTexture();
    m_TextureMap.clear();
}
