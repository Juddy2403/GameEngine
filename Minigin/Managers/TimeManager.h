#pragma once
#include <chrono>

#include "Singleton.h"

namespace GameEngine
{
	class TimeManager final : public GameEngine::Singleton<TimeManager>
	{
	public:
		static float GetElapsed();
		[[nodiscard]] std::chrono::high_resolution_clock::time_point GetCurrent() const;
		void Update();
	private:
		friend class GameEngine::Singleton<TimeManager>;
		TimeManager() = default;
		static float m_ElapsedTime;
		const float m_ElapsedTimeCap{ 0.05f };
		std::chrono::high_resolution_clock::time_point m_Previous{std::chrono::high_resolution_clock::now() };
		std::chrono::high_resolution_clock::time_point m_Current{};
	};
}

