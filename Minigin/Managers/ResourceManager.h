#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include "Singleton.h"
#include "../Renderable/Texture2D.h"

namespace GameEngine
{
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		[[nodiscard]] Texture2D* LoadTexture(const std::string& file);
		[[nodiscard]] Texture2D* LoadTexture(std::unique_ptr<Texture2D>&& texture);
		[[nodiscard]] std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::unordered_map<std::string,std::unique_ptr<Texture2D>> m_TextureMap;
		//for the textures that are init with a texture2d obj (for eg text textures)
		std::vector<std::unique_ptr<Texture2D>> m_TextureVec;
		std::string m_dataPath;
	};
}
