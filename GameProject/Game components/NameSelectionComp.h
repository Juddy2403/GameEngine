#pragma once
#include <array>
#include <string>
#include <vector>

#include "Components/Component.h"

namespace GameEngine
{
    class TextComponent;
}
class NameSelectionComp: public GameEngine::Component
{
public:
    explicit NameSelectionComp(GameEngine::GameObject* gameObj);
    void AddTextComponent(GameEngine::TextComponent* textComp);
    std::string GetName() const { return m_Name; }
    bool MoveRight();
    void MoveUp();
    void MoveDown();
private:
    int m_CurrentIdx = 0;
    std::string m_Name;
    std::vector<GameEngine::TextComponent*> m_TextComponents;
    
};
