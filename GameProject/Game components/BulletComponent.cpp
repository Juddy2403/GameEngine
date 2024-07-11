#include "BulletComponent.h"

#include "DataStructs.h"
#include "Components/SpriteComponent.h"
#include "Subjects/GameObject.h"
#include "Managers/TimeManager.h"

BulletComponent::BulletComponent(GameEngine::GameObject* gameObj, int playerID,
    GameEngine::SpriteComponent* spriteComponent):
    Component(gameObj),
    m_PlayerID(playerID),
    m_SpriteComponent(spriteComponent)
{}

void BulletComponent::Update()
{
    int bulletYPos = m_SpriteComponent->m_DestRect.y + m_SpriteComponent->m_DestRect.h;
    
    if (bulletYPos <= 0)
    {
        GetGameObjParent()->Notify( static_cast<int>(GameEvent::bulletOutOfBounds),
            static_cast<int>(ObserverIdentifier::bullet));
        return;
    }

    GetGameObjParent()->GetLocalTransform().Translate(GameEngine::TimeManager::GetElapsed() * m_Velocity);
}
