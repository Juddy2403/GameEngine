#include "EnemyObserver.h"

#include "DataStructs.h"
#include "EventData.h"
#include "Galaga.h"
#include "ScoreManager.h"
#include "Game components/BulletComponent.h"
#include "Game components/Enemy components/EnemyComponent.h"
#include "Sound/ServiceLocator.h"
#include "Subjects/GameObject.h"

void EnemyObserver::Notify(GameEngine::Subject* subject, int event
    , [[maybe_unused]] GameEngine::EventData* eventData)
{
    GameEngine::GameObject* actor = dynamic_cast<GameEngine::GameObject*>(subject);
    switch (static_cast<GameEvent>(event))
    {
    case GameEvent::collision:
    {
        const auto collisionData = reinterpret_cast<GameEngine::CollisionData*>(eventData);
        if (collisionData->pOtherCollider->GetID() == static_cast<int>(GameId::bullet))
        {
            auto enemyComp = actor->GetComponent<EnemyComponent>();
            if(!enemyComp->HasCurrentState()) return;
            if (bool hasDied = enemyComp->HasBeenHit())
            {
                ScoreManager::AddScore(enemyComp->GetEnemyID());
                subject->NotifyAll(static_cast<int>(GameEvent::died));
                //play sound
                auto enemyID = actor->GetComponent<EnemyComponent>()->GetEnemyID();

                if (enemyID == EnemyId::bossGalagaDiving || enemyID == EnemyId::bossGalaga)
                    GameEngine::ServiceLocator::GetSoundSystem().PlaySound(static_cast<GameEngine::SoundId>(SoundId::bossDeath), Galaga::volume);
                else GameEngine::ServiceLocator::GetSoundSystem().PlaySound(static_cast<GameEngine::SoundId>(SoundId::enemyDeath), Galaga::volume);
            }
        }
    }
        break;
    default: break;
    }
}
