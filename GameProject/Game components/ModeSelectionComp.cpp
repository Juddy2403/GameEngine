#include "ModeSelectionComp.h"

#include "Components/TextComponent.h"

ModeSelectionComp::ModeSelectionComp(GameEngine::GameObject* gameObj)
: Component(gameObj) 
{}

void ModeSelectionComp::AddTextComponent(GameEngine::TextComponent* textComp, GameMode mode)
{
    if(m_TextComponents.empty()) textComp->SetColor({255,255,0,255});
    m_Modes.emplace_back(mode);
    m_TextComponents.push_back(textComp);
}
void ModeSelectionComp::MoveUp()
{
    if(m_CurrentIdx == 0) return;
    m_TextComponents[m_CurrentIdx]->SetColor({255,255,255,255});
    --m_CurrentIdx;
    m_TextComponents[m_CurrentIdx]->SetColor({255,255,0,255});
}
void ModeSelectionComp::MoveDown()
{ 
    if(m_CurrentIdx == std::ssize(m_TextComponents)-1) return;
    m_TextComponents[m_CurrentIdx]->SetColor({255,255,255,255});
    ++m_CurrentIdx;
    m_TextComponents[m_CurrentIdx]->SetColor({255,255,0,255});
}
