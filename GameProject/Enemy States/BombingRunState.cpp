#include "BombingRunState.h"
#include <queue>
#include "DataStructs.h"
#include "IdleState.h"
#include "Game components/Enemy components/EnemyComponent.h"

void BombingRunState::Enter(EnemyComponent* enemyComponent)
{
    std::queue<PathData> pathDataQueue;
    PathData pathData;

    enemyComponent->GetGameObjParent()->Notify(static_cast<int>(GameEvent::bulletShot),
        static_cast<int>(ObserverIdentifier::enemyAttack));
    // Initial upward movement
    pathData.destination = glm::vec2(enemyComponent->GetGameObjParent()->GetPosition()) + glm::vec2(0, -50);
    pathDataQueue.push(pathData);

    // Loop movement
    pathData = {};
    pathData.isRotating = true;
    pathData.isRotatingClockwise = static_cast<bool>(rand() % 2);
    pathData.centerOfRotation = glm::vec2(enemyComponent->GetGameObjParent()->GetPosition()) + glm::vec2{ 0,20 };
    pathData.totalRotationAngle = 3.49066f;
    pathDataQueue.push(pathData);

    // Dive towards the player
    pathData = {};
    const glm::vec2 playerPos = enemyComponent->GetPlayerComponent()->GetGameObjParent()->GetPosition();
    pathData.destination = playerPos + glm::vec2{ 0,-50 };
    pathDataQueue.push(pathData);

    // Loop movement
    pathData = {};
    pathData.isRotating = true;
    pathData.isRotatingClockwise = static_cast<bool>(rand() % 2);
    pathData.centerOfRotation = playerPos;
    pathData.totalRotationAngle = 4.88692f;
    pathDataQueue.push(pathData);

    pathData = {};
    pathData.destination = glm::vec2(enemyComponent->GetFormationPosition());
    pathDataQueue.push(pathData);

    // Set the trajectory
    m_BombingTrajectory->SetPathData(pathDataQueue, enemyComponent->GetGameObjParent()->GetPosition());
}
std::unique_ptr<EnemyState> BombingRunState::Update(EnemyComponent* enemyComponent)
{
    if (!m_NextBulletShot)
    {
        m_AccumTime += GameEngine::TimeManager::GetElapsed();
        if (m_AccumTime >= m_TimeTillNextBulletShot)
        {
            m_NextBulletShot = true;
            enemyComponent->GetGameObjParent()->Notify(static_cast<int>(GameEvent::bulletShot),
                static_cast<int>(ObserverIdentifier::enemyAttack));
        }
    }
    if (enemyComponent->UpdateTrajectory(*m_BombingTrajectory)) return std::make_unique<IdleState>();
    return nullptr;
}
