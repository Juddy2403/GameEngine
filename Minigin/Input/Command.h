#pragma once
#include <glm/glm.hpp>

namespace GameEngine
{
	class GameObject;

	class Command
	{
	private:
		bool m_IsDestroyed{false};
	protected:
		GameObject* m_Actor;
	public:
		enum class ExecuteOn
		{
			keyPressed,
			keyUp,
			keyDown
		};
		
		explicit Command(GameObject* actor);
		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;
		virtual ~Command() = default;

		[[nodiscard]] virtual ExecuteOn ExecuteOnKeyState() const = 0;
		virtual void Execute() = 0;
		void SetDestroyedFlag() { m_IsDestroyed = true; }
		[[nodiscard]] bool IsDestroyed() const { return m_IsDestroyed; }
	};

	class Move final: public Command {
	public:
		Move(const Move& other) = delete;
		Move(Move&& other) noexcept = delete;
		Move& operator=(const Move& other) = delete;
		Move& operator=(Move&& other) noexcept = delete;
		
		explicit Move(GameObject* actor,const glm::vec2& direction, int speed = 100);
		~Move() override;
		void Execute() override;
		[[nodiscard]] ExecuteOn ExecuteOnKeyState() const override;
	private:
		glm::vec2 m_Direction{};
		int m_Speed{};
	};
	
}


