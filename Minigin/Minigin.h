#pragma once
#include <string>
#include <functional>
#include <SDL.h>

namespace GameEngine
{
	static constexpr SDL_Rect g_WindowRect{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 672, 612};

	class Minigin final
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}