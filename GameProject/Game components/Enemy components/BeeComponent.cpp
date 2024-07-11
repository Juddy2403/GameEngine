#include "BeeComponent.h"

#include "Enemy States/BombingRunState.h"
#include "Game observers/EnemyAIManager.h"

BeeComponent::BeeComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent, PlayerComponent* playerComponent):
    EnemyComponent(gameObj, spriteComponent,playerComponent)
{}

void BeeComponent::GetInAttackState()
{
    m_CurrentState->Exit(this);
    m_CurrentState = std::make_unique<BombingRunState>();
    m_CurrentState->Enter(this);
}
EnemyId BeeComponent::GetEnemyID() const
{
    if(!m_CurrentState) return EnemyId::bee;
    if (m_CurrentState->IsDiving()) return EnemyId::beeDiving;
    return EnemyId::bee;
}
