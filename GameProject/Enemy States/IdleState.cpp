#include "IdleState.h"

#include "Game components/FormationComponent.h"
#include "Game components/Enemy components/EnemyComponent.h"

void IdleState::UpdateBackToFormationTrajectory(EnemyComponent* enemyComponent) const
{
    std::queue<PathData> pathDataQueue;
    PathData pathData;
    pathData.destination = glm::vec2(enemyComponent->GetFormationPosition()) + glm::vec2(FormationComponent::GetOffset(),0);
    pathDataQueue.push(pathData);
    m_BackToFormationTrajectory->SetPathData(pathDataQueue, enemyComponent->GetGameObjParent()->GetPosition());
}
void IdleState::GotInFormation(EnemyComponent* enemyComponent) {
    enemyComponent->GetGameObjParent()->NotifyAll(static_cast<int>(GameEvent::gotInFormation));
    enemyComponent->GetRotatingSprite()->RotateSpriteInDirection({0,1});

}
void IdleState::Enter(EnemyComponent* enemyComponent)
{
    if(!TrajectoryMath::ArePositionsEqual(enemyComponent->GetGameObjParent()->GetPosition(),
        enemyComponent->GetFormationPosition()+glm::ivec2{FormationComponent::GetOffset(),0}))
    {
        m_BackToFormationTrajectory = std::make_unique<Trajectory>();
        UpdateBackToFormationTrajectory(enemyComponent);
        return;
    }
    GotInFormation(enemyComponent);
}
std::unique_ptr<EnemyState> IdleState::Update(EnemyComponent* enemyComponent)
{
    if(m_BackToFormationTrajectory != nullptr)
    {
        if (enemyComponent->UpdateTrajectory(*m_BackToFormationTrajectory))
        {
            m_BackToFormationTrajectory = nullptr;
            GotInFormation(enemyComponent);
            return nullptr;
        }
        UpdateBackToFormationTrajectory(enemyComponent);

        return nullptr;
    }
    glm::vec2 formationPos = enemyComponent->GetFormationPosition();
    formationPos.x += FormationComponent::GetOffset();
    enemyComponent->GetGameObjParent()->SetPosition({ formationPos,0 });
    return nullptr;
}
void IdleState::Exit(EnemyComponent* enemyComponent)
{
    enemyComponent->GetGameObjParent()->NotifyAll(static_cast<int>(GameEvent::leftFormation));
}