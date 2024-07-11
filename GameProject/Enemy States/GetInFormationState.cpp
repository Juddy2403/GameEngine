#include "GetInFormationState.h"

#include "IdleState.h"
#include "Game components/Enemy components/EnemyComponent.h"
#include "Managers/TimeManager.h"

const float GetInFormationState::m_TimeInBetween = 0.14f;

std::unique_ptr<EnemyState> GetInFormationState::Update(EnemyComponent* enemyComponent)
{
    m_WaitTime = m_TimeInBetween * enemyComponent->m_SetOutTurn;
    m_AccumWaitTime += GameEngine::TimeManager::GetElapsed();
    if (m_AccumWaitTime < m_WaitTime) return nullptr;
    m_HasSetOut = true;
    if (enemyComponent->UpdateTrajectory(enemyComponent->GetFormationTrajectory())) return std::make_unique<IdleState>();
    return nullptr;
}