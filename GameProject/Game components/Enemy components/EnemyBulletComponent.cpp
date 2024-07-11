#include "EnemyBulletComponent.h"

#include "DataStructs.h"
#include "Minigin.h"
#include "Managers/TimeManager.h"
#include "Subjects/GameObject.h"

EnemyBulletComponent::EnemyBulletComponent(GameEngine::GameObject* gameObj, const glm::vec2& direction):
Component(gameObj), m_Direction(direction) {}
void EnemyBulletComponent::Update()
{
    int bulletYPos = GetGameObjParent()->GetIntPosition().y;
    
    if (bulletYPos > GameEngine::g_WindowRect.h)
    {
        GetGameObjParent()->Notify( static_cast<int>(GameEvent::bulletOutOfBounds),
            static_cast<int>(ObserverIdentifier::enemyAttack));
        return;
    }

    GetGameObjParent()->GetLocalTransform().Translate(GameEngine::TimeManager::GetElapsed() * m_Direction * m_Speed);
}
