#include "ButterflyComponent.h"

#include "Enemy States/BossShootingBeamState.h"
#include "Enemy States/ButterflyBombingRunState.h"
#include "Subjects/GameObject.h"

ButterflyComponent::ButterflyComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent,PlayerComponent* playerComponent):
    EnemyComponent(gameObj, spriteComponent, playerComponent)
{}

void ButterflyComponent::GetInAttackState()
{
    m_CurrentState->Exit(this);
    m_CurrentState = std::make_unique<ButterflyBombingRunState>();
    m_CurrentState->Enter(this);
}

EnemyId ButterflyComponent::GetEnemyID() const
{
    if(!m_CurrentState) return EnemyId::butterfly;
    if (m_CurrentState->IsDiving()) return EnemyId::butterflyDiving;
    return EnemyId::butterfly;
}