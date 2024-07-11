#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <XInput.h>
#include "Controller.h"
#include <iostream>
#include <unordered_map>

class GameEngine::Controller::XInput
{
    const std::unordered_map<ControllerInputKey, std::uint32_t> keyMappings = {
        { ControllerInputKey::dpadUp, XINPUT_GAMEPAD_DPAD_UP },
        { ControllerInputKey::dpadLeft,XINPUT_GAMEPAD_DPAD_LEFT },
        { ControllerInputKey::dpadDown,XINPUT_GAMEPAD_DPAD_DOWN },
        { ControllerInputKey::dpadRight,XINPUT_GAMEPAD_DPAD_RIGHT },
        { ControllerInputKey::X,XINPUT_GAMEPAD_X },
        { ControllerInputKey::Y,XINPUT_GAMEPAD_Y },
        { ControllerInputKey::A,XINPUT_GAMEPAD_A },
        { ControllerInputKey::B, XINPUT_GAMEPAD_B } };
    
    unsigned int m_ControllerIdx{ 0 };
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};
    unsigned int m_ButtonsPressedThisFrame{};
    unsigned int m_ButtonsReleasedThisFrame{};
public:
    explicit XInput(unsigned int controllerIdx): m_ControllerIdx{ controllerIdx } {}
    XInput(const XInput& other) = delete; 
    XInput(XInput&& other) = delete;
    XInput& operator=(const XInput& other) = delete;
    XInput& operator=(XInput&& other) = delete;
    ~XInput() = default;

    void ProcessControllerInput()
    {
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        if (const auto result = XInputGetState(m_ControllerIdx, &m_CurrentState); result != ERROR_SUCCESS)
        {
            switch (result)
            {
            case ERROR_DEVICE_NOT_CONNECTED:
                std::cerr << "Controller error! Device not connected! \n";
                break;
            case ERROR_NOT_SUPPORTED:
                std::cerr << "Controller error! Not supported! \n";
                break;
            default:
                std::cerr << "Controller error! \n";
                break;
            }
        }

        const int buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
        m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
    }
    [[nodiscard]] bool IsKeyDown(ControllerInputKey button) const
    {
        try
        {
            return m_ButtonsPressedThisFrame & keyMappings.at(button);
        }
        catch (const std::out_of_range& e)
        {
            std::cerr <<"Out of range error: "<<e.what()<<" Controller.cpp\n";
        }
        return false;
    }
    [[nodiscard]] bool IsKeyUp(ControllerInputKey button) const
    {
        try
        {
            return m_ButtonsReleasedThisFrame & keyMappings.at(button);
        }
        catch (const std::out_of_range& e)
        {
            std::cerr <<"Out of range error: "<<e.what()<<" Controller.cpp\n";
        }
        return false;
    }
    [[nodiscard]] bool IsKeyPressed(ControllerInputKey button) const
    {
        try
        {
            return m_CurrentState.Gamepad.wButtons & keyMappings.at(button);
        }
        catch (const std::out_of_range& e)
        {
            std::cerr <<"Out of range error: "<<e.what()<<" Controller.cpp\n";
        }
        return false;
    }
};

GameEngine::Controller::Controller(unsigned int controllerIdx):
    m_pXInput{ std::make_unique<XInput>(controllerIdx) }
{}

GameEngine::Controller::~Controller() {}

void GameEngine::Controller::ProcessControllerInput() const
{
    m_pXInput->ProcessControllerInput();
}

bool GameEngine::Controller::IsKeyDown(ControllerInputKey inputKey) const
{
   return m_pXInput->IsKeyDown(inputKey);
}
bool GameEngine::Controller::IsKeyPressed(ControllerInputKey inputKey) const
{
    return m_pXInput->IsKeyPressed(inputKey);
}

bool GameEngine::Controller::IsKeyUp(ControllerInputKey inputKey) const
{
    return m_pXInput->IsKeyUp(inputKey);
}
