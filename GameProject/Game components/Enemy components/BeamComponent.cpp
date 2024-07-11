#include "BeamComponent.h"

#include "DataStructs.h"
#include "Components/SpriteComponent.h"
#include "Subjects/GameObject.h"

BeamComponent::BeamComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent, EnemyComponent* parentComp):
    Component(gameObj),
    m_ParentComp(parentComp),
    m_SpriteComponent(spriteComponent) {}

void BeamComponent::Update()
{
    if (m_IsBeamRetracting)
    {
        if (m_SpriteComponent->m_SpriteInfo.m_CurrentCol == m_SpriteComponent->m_SpriteInfo.m_NrOfCols - 1)
        {
            if (m_CurrentRow == 0)
            {
                GetGameObjParent()->Notify(static_cast<int>(GameEvent::beamRetracted),
                    static_cast<int>(ObserverIdentifier::enemyAttack));
                return;
            }
            m_SpriteComponent->m_SpriteInfo.m_CurrentCol = 0;
            m_SpriteComponent->m_SpriteInfo.m_StartPos.y -= m_SpriteComponent->m_SpriteInfo.m_Height + m_SpriteComponent->m_SpriteInfo.m_Spacing;
            --m_CurrentRow;
        }
    }
    else if (m_SpriteComponent->m_SpriteInfo.m_CurrentCol == m_SpriteComponent->m_SpriteInfo.m_NrOfCols - 1 &&
        m_SpriteComponent->m_SpriteInfo.m_CurrentRow == m_SpriteComponent->m_SpriteInfo.m_NrOfRows - 1)
    {
        m_IsBeamRetracting = true;
        m_CurrentRow = m_SpriteComponent->m_SpriteInfo.m_NrOfRows - 2;
        m_SpriteComponent->m_SpriteInfo.m_CurrentCol = 0;
        m_SpriteComponent->m_SpriteInfo.m_CurrentRow = 0;
        m_SpriteComponent->m_SpriteInfo.m_NrOfCols = 3;
        m_SpriteComponent->m_SpriteInfo.m_NrOfRows = 1;
        m_SpriteComponent->m_SpriteInfo.m_StartPos.y += (m_SpriteComponent->m_SpriteInfo.m_Height + m_SpriteComponent->m_SpriteInfo.m_Spacing) * m_CurrentRow;
    }
}
bool BeamComponent::IsBeamActive() const
{
    BossGalagaComponent* bossComp = dynamic_cast<BossGalagaComponent*>(m_ParentComp);
    return bossComp->HasCapturedFighter();
}
