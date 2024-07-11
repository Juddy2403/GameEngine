#include "CollisionComponent.h"

#include <iostream>

#include "../EventData.h"
#include "../Managers/CollisionManager.h"
#include "../Subjects/GameObject.h"
#include "../Subjects/Subject.h"

using namespace GameEngine;
CollisionComponent::CollisionComponent(GameObject* gameObj,SDL_Rect collisionRect):
    Component(gameObj),
    m_CollisionRect(collisionRect)
{
    m_LastPosition = GetGameObjParent()->GetIntPosition();
}
const SDL_Rect& CollisionComponent::GetCollisionRect() const
{
    return m_CollisionRect;
}
bool CollisionComponent::IsColliding(CollisionComponent* other) const
{
    return SDL_HasIntersection(&m_CollisionRect, &other->m_CollisionRect);
}
void CollisionComponent::CollidedWith(CollisionComponent* other) const
{
    CollisionData data;
    data.pOtherCollider = other->GetGameObjParent();
    if(!GetGameObjParent()->IsDestroyed()) GetGameObjParent()->NotifyAll(static_cast<int>(EngineGameEvent::collision),&data);
}

void CollisionComponent::Update()
{
    auto pos = GetGameObjParent()->GetIntPosition();
    m_CollisionRect.x += pos.x - m_LastPosition.x;
    m_CollisionRect.y += pos.y - m_LastPosition.y;
    m_LastPosition = {pos.x,pos.y};
}
