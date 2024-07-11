#include "FormationComponent.h"
#include <iostream>

#include "Subjects/GameObject.h"
#include "Managers/TimeManager.h"

bool FormationComponent::m_IsUpdating = false;
float FormationComponent::m_Offset = 0;
int FormationComponent::m_Direction = 1;

FormationComponent::FormationComponent(GameEngine::GameObject* gameObj):
    Component(gameObj)
{
    m_IsUpdating = false;
}
void FormationComponent::Update()
{
    if (m_IsUpdating)
    {
        m_Offset += m_Direction * m_Speed * GameEngine::TimeManager::GetElapsed();
        if (m_Offset <= -m_OffsetLimit) m_Direction = 1;
        else if (m_Offset >= m_OffsetLimit) m_Direction = -1;
    }
}
float FormationComponent::GetOffset()
{
     return m_Offset;
}
