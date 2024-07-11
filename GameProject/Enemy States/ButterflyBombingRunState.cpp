#include "ButterflyBombingRunState.h"

#include "Minigin.h"
#include "Game components/Enemy components/EnemyComponent.h"

void ButterflyBombingRunState::Enter(EnemyComponent* enemyComponent)
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
    pathData.centerOfRotation = glm::vec2(enemyComponent->GetGameObjParent()->GetPosition())+ glm::vec2{ 0,20 };
    pathData.totalRotationAngle = 2.49066f;
    pathDataQueue.push(pathData);

    pathData = {};
    pathData.destination = glm::vec2(enemyComponent->GetGameObjParent()->GetPosition()) + glm::vec2(0, 100);
    pathDataQueue.push(pathData);

    pathData = {};
    pathData.destination = glm::vec2(enemyComponent->GetGameObjParent()->GetPosition()) + glm::vec2(50, 150);
    pathDataQueue.push(pathData);

    pathData = {};
    pathData.isRotating = true;
    pathData.isRotatingClockwise = static_cast<bool>(rand() % 2);
    pathData.centerOfRotation = glm::vec2(enemyComponent->GetGameObjParent()->GetPosition()) + glm::vec2(70, 170);
    pathData.totalRotationAngle = 2.49066f;
    pathDataQueue.push(pathData);

    // Dive towards the player
    pathData = {};
    const glm::vec2 playerPos = enemyComponent->GetPlayerComponent()->GetGameObjParent()->GetPosition();
    pathData.destination = playerPos + glm::vec2{ 0,-50 };
    pathDataQueue.push(pathData);

    // Dive outside of the screen
    if(playerPos.x >= GameEngine::g_WindowRect.w/2)
    {
        // Loop movement
        pathData = {};
        pathData.isRotating = true;
        pathData.isRotatingClockwise = false;
        pathData.centerOfRotation = playerPos;
        pathData.totalRotationAngle = 4.7f;
        pathDataQueue.push(pathData);
        
        pathData = {};
        pathData.destination = {GameEngine::g_WindowRect.w + 100, playerPos.y};
        pathDataQueue.push(pathData);
        
        pathData = {};
        pathData.destination = {GameEngine::g_WindowRect.w + 100, - 100};
        pathDataQueue.push(pathData);
    }
    else
    {
        // Loop movement
        pathData = {};
        pathData.isRotating = true;
        pathData.isRotatingClockwise = true;
        pathData.centerOfRotation = playerPos;
        pathData.totalRotationAngle = 4.7f;
        pathDataQueue.push(pathData);
        
        pathData = {};
        pathData.destination = {-100, playerPos.y};
        pathDataQueue.push(pathData);
        
        pathData = {};
        pathData.destination = {- 100, - 100};
        pathDataQueue.push(pathData);
    }

    pathData = {};
    pathData.destination = glm::vec2(enemyComponent->GetFormationPosition());
    pathDataQueue.push(pathData);

    // Set the trajectory
    m_BombingTrajectory->SetPathData(pathDataQueue, enemyComponent->GetGameObjParent()->GetPosition());
}