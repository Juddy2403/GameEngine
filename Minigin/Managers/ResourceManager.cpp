#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Minigin/Renderable/Renderer.h"
#include "Minigin/Renderable/Font.h"

void GameEngine::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

GameEngine::Texture2D* GameEngine::ResourceManager::LoadTexture(const std::string& file) 
{
	const auto fullPath = m_dataPath + file;
	if(m_TextureMap.contains(fullPath)) return m_TextureMap.at(fullPath).get();
	
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	m_TextureMap[fullPath] = std::make_unique<Texture2D>(texture);
	
	return m_TextureMap.at(fullPath).get();
}
GameEngine::Texture2D* GameEngine::ResourceManager::LoadTexture(std::unique_ptr<Texture2D>&& texture)
{
	m_TextureVec.emplace_back(std::move(texture));
	return m_TextureVec.back().get();
}

std::shared_ptr<GameEngine::Font> GameEngine::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}
