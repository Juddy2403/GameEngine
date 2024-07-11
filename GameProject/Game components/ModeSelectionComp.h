#pragma once
#include <vector>

#include "Components/Component.h"

enum class GameMode;
namespace GameEngine
{
    class TextComponent;
}
class ModeSelectionComp final : public GameEngine::Component
{
public:
    explicit ModeSelectionComp(GameEngine::GameObject* gameObj);
    void AddTextComponent(GameEngine::TextComponent* textComp, GameMode mode);
    void MoveUp();
    void MoveDown();
    GameMode GetSelectedMode() const { return m_Modes[m_CurrentIdx]; }
private:
    int m_CurrentIdx = 0;
    std::vector<GameMode> m_Modes;
    std::vector<GameEngine::TextComponent*> m_TextComponents;
};
