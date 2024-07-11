#include "EnemyComponent.h"
#include "Enemy States/GetInFormationState.h"
#include "Enemy States/IdleState.h"
#include "Game components/FormationComponent.h"
#include "Game observers/EnemyAIManager.h"
#include "Game observers/FormationObserver.h"
#include "Subjects/GameObject.h"

EnemyComponent::EnemyComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent, PlayerComponent* playerComponent):
    Component(gameObj),
    m_PlayerComponent(playerComponent),
    m_CurrentState(nullptr),
    m_RotatingSprite(std::make_unique<RotatingSprite>(spriteComponent))
{
    EnemyAIManager::AddEnemy(this);
}
void EnemyComponent::GetInIdleState()
{
    m_CurDirection = { 0,1 };
    m_CurrentState->Exit(this);
    m_CurrentState = std::make_unique<IdleState>();
    m_CurrentState->Enter(this);
}
void EnemyComponent::SetFormationPosition(const glm::ivec2& formationPos)
{
    m_FormationPosition = formationPos;
}
void EnemyComponent::SetFormationTrajectory(const std::queue<PathData>& pathDataQueue)
{
    m_FormationTrajectory.SetPathData(pathDataQueue, GetGameObjParent()->GetPosition());
}
bool EnemyComponent::HasSetOut() const
{
    if(auto getInFormationState = dynamic_cast<GetInFormationState*>(m_CurrentState.get()))
        return getInFormationState->HasSetOut();
    return false;
}

void EnemyComponent::Update()
{
    if (m_CurrentState)
    {
        if (auto nextState = m_CurrentState->Update(this))
        {
            m_CurrentState->Exit(this);
            m_CurrentState = std::move(nextState);
            m_CurrentState->Enter(this);
        }
    }
    else if (FormationObserver::GetCurrentStage() == m_Stage)
    {
        FormationObserver::EnemySetOut();
        m_CurrentState = std::make_unique<GetInFormationState>();
        m_CurrentState->Enter(this);
    }
}
void EnemyComponent::Died()
{
    if(m_CurrentState) m_CurrentState->Exit(this);
    EnemyAIManager::RemoveEnemy(this);
    GetGameObjParent()->SetDestroyedFlag();
}

bool EnemyComponent::UpdateTrajectory(Trajectory& trajectory) 
{
    if (trajectory.IsComplete()) return true;
    const glm::vec2 currentPos = GetGameObjParent()->GetPosition();
    auto [newPos, hasDirectionChanged] = trajectory.Update(GetSpeed(), currentPos);
    if (hasDirectionChanged)
    {
        m_CurDirection = trajectory.GetDirection();
        m_RotatingSprite->RotateSpriteInDirection(trajectory.GetDirection());
    }
    GetGameObjParent()->SetPosition({ newPos,0 });
    return false;
}
