#include "InputManager.h"

#include <algorithm>
#include <ranges>

void GameEngine::InputManager::UnbindRemovedCommands()
{
    std::erase_if(m_pKeyboardCommands, [](const auto& pair) {
        return pair.second == nullptr || pair.second->IsDestroyed();
    });
    for (auto& controllerCommands : m_pControllerCommands)
    {
        std::erase_if(controllerCommands, [](const auto& pair) {
            return pair.second == nullptr || pair.second->IsDestroyed();
        });
    }
}
bool GameEngine::InputManager::ProcessInput()
{
    if (!ProcessKeyboardInput()) return false;
    ProcessControllerInput();
    if (m_AreElemsToUnbind)
    {
        m_AreElemsToUnbind = false;
        UnbindRemovedCommands();
    }
    return true;
}


void GameEngine::InputManager::ProcessControllerInput()
{
    for (size_t i = 0; i < g_maxControllerCount; i++)
    {
        if (m_pControllers[i] == nullptr) continue;
        m_pControllers[i]->ProcessControllerInput();

        //TODO: perhaps create an event queue with the inputs received
        for (const auto& [inputKey, command] : m_pControllerCommands[i])
        {
            if (command == nullptr) continue;
            if (command->IsDestroyed()) continue;
            switch (command->ExecuteOnKeyState())
            {
            case Command::ExecuteOn::keyPressed:
                if (m_pControllers[i]->IsKeyPressed(inputKey))
                    command->Execute();
                break;
            case Command::ExecuteOn::keyUp:
                if (m_pControllers[i]->IsKeyUp(inputKey)) command->Execute();
                break;
            case Command::ExecuteOn::keyDown:
                if (m_pControllers[i]->IsKeyDown(inputKey)) command->Execute();
                break;
            }
        }
    }
}
bool GameEngine::InputManager::ProcessKeyboardInput()
{
    if (!m_pKeyboard->ProcessKeyboardInput()) return false;
    //TODO: perhaps create an event queue with the inputs received
    for (const auto& [inputKey, command] : m_pKeyboardCommands)
    {
        if (command == nullptr) continue;
        if (command->IsDestroyed()) continue;
        switch (command->ExecuteOnKeyState())
        {
        case Command::ExecuteOn::keyPressed:
        {
            if (m_pKeyboard->IsKeyPressed(inputKey)) command->Execute();
        }
        break;
        case Command::ExecuteOn::keyUp:
        {
            if (m_pKeyboard->IsKeyUp(inputKey)) command->Execute();
        }
        break;
        case Command::ExecuteOn::keyDown:
        {
            if (m_pKeyboard->IsKeyDown(inputKey)) command->Execute();
        }
        break;
        }
    }
    return true;
}

void GameEngine::InputManager::BindCommand(KeyboardInputKey inputKey, std::unique_ptr<Command>&& command)
{
    m_pKeyboardCommands[inputKey] = std::move(command);
}
void GameEngine::InputManager::BindCommand(ControllerInputKey inputKey, std::unique_ptr<Command>&& command, int controllerIdx)
{
    if (m_pControllers[controllerIdx] == nullptr)
        m_pControllers[controllerIdx] = std::make_unique<Controller>(controllerIdx);

    m_pControllerCommands[controllerIdx][inputKey] = std::move(command);
}

void GameEngine::InputManager::UnbindCommand(KeyboardInputKey inputKey)
{
    m_AreElemsToUnbind = true;
    if(m_pKeyboardCommands[inputKey]) m_pKeyboardCommands[inputKey]->SetDestroyedFlag();
}

void GameEngine::InputManager::UnbindCommand(ControllerInputKey inputKey, int controllerIdx)
{
    m_AreElemsToUnbind = true;
    if(m_pControllerCommands[controllerIdx][inputKey]) m_pControllerCommands[controllerIdx][inputKey]->SetDestroyedFlag();
}
