#pragma once
#include <memory>

namespace GameEngine {
	constexpr size_t g_maxControllerCount{4};

	enum class ControllerInputKey {
		dpadUp,
		dpadLeft,
		dpadDown,
		dpadRight,
		X,
		Y,
		A,
		B
	};
	
	class Controller final
	{
	public:
		explicit Controller(unsigned int controllerIdx);
		~Controller();
		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		void ProcessControllerInput() const;

		[[nodiscard]] bool IsKeyDown(ControllerInputKey inputKey) const;
		[[nodiscard]] bool IsKeyPressed(ControllerInputKey inputKey) const;
		[[nodiscard]] bool IsKeyUp(ControllerInputKey inputKey) const;

	private:
		class XInput;
		std::unique_ptr<XInput> m_pXInput;
	};
}


