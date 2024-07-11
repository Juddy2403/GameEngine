#include "NameSelectionComp.h"

#include "Components/TextComponent.h"
NameSelectionComp::NameSelectionComp(GameEngine::GameObject* gameObj): Component(gameObj),
                                                                       m_Name{"AAA"}{}

void NameSelectionComp::AddTextComponent(GameEngine::TextComponent* textComp)
{
    if(m_TextComponents.empty()) textComp->SetColor({255,255,0,255});
    textComp->SetText("A");
    m_TextComponents.push_back(textComp);
}
bool NameSelectionComp::MoveRight()
{
    if(m_CurrentIdx == 3)
    {
        m_TextComponents[m_CurrentIdx-1]->SetColor({255,255,255,255});
        return true;
    }

    m_TextComponents[m_CurrentIdx]->SetColor({255,255,255,255});
    ++m_CurrentIdx;
    if(m_CurrentIdx != 3) m_TextComponents[m_CurrentIdx]->SetColor({255,255,0,255});
    return false;
}
void NameSelectionComp::MoveUp()
{
    if(m_Name[m_CurrentIdx] == 'Z') m_Name[m_CurrentIdx] = 'A';
    else ++m_Name[m_CurrentIdx];
    m_TextComponents[m_CurrentIdx]->SetText(std::string(1, m_Name[m_CurrentIdx]));
}
void NameSelectionComp::MoveDown()
{
    if(m_Name[m_CurrentIdx] == 'A') m_Name[m_CurrentIdx] = 'Z';
    else --m_Name[m_CurrentIdx];
    m_TextComponents[m_CurrentIdx]->SetText(std::string(1, m_Name[m_CurrentIdx]));
}
