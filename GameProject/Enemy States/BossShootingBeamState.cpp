#include "BossShootingBeamState.h"

#include "Galaga.h"
#include "Minigin.h"
#include "Game components/Enemy components/EnemyComponent.h"
#include "Sound/ServiceLocator.h"

void BossShootingBeamState::Enter(EnemyComponent* enemyComponent)
{
    std::queue<PathData> pathDataQueue;
    PathData pathData;

    const auto enemyPos = enemyComponent->GetGameObjParent()->GetPosition();

    enemyComponent->GetGameObjParent()->Notify(static_cast<int>(GameEvent::bulletShot),
        static_cast<int>(ObserverIdentifier::enemyAttack));
    // Initial upward movement
    pathData.destination = glm::vec2(enemyPos) + glm::vec2(0, -50);
    pathDataQueue.push(pathData);

    // Loop movement
    pathData = {};
    pathData.isRotating = true;
    pathData.isRotatingClockwise = static_cast<bool>(rand() % 2);
    pathData.centerOfRotation = glm::vec2(enemyPos) + glm::vec2{ 0,20 };
    pathData.totalRotationAngle = 3.49066f;
    pathDataQueue.push(pathData);

    // Dive towards the player
    pathData = {};
    const glm::vec2 playerPos = enemyComponent->GetPlayerComponent()->GetGameObjParent()->GetPosition();
    pathData.destination = playerPos + glm::vec2{ 0,(enemyPos.y - playerPos.y) / 2 + 100 };
    pathDataQueue.push(pathData);

    // Set the trajectory
    m_BeamTrajectory->SetPathData(pathDataQueue, enemyComponent->GetGameObjParent()->GetPosition());
}
std::unique_ptr<EnemyState> BossShootingBeamState::Update(EnemyComponent* enemyComponent)
{
    if (m_IsShootingBeam) return nullptr;
    if (enemyComponent->UpdateTrajectory(*m_BeamTrajectory))
    {
        enemyComponent->GetGameObjParent()->Notify(static_cast<int>(GameEvent::bossShotBeam),
            static_cast<int>(ObserverIdentifier::enemyAttack));
        enemyComponent->GetRotatingSprite()->RotateSpriteInDirection({ 0,1 });
        enemyComponent->SetCurDirection({ 0,1 });
        GameEngine::ServiceLocator::GetSoundSystem().PlaySound(static_cast<GameEngine::SoundId>(SoundId::tractorBeam), Galaga::volume);
        m_IsShootingBeam = true;
    }
    return nullptr;
}
