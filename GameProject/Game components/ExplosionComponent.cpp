#include "ExplosionComponent.h"

#include "Components/SpriteComponent.h"
#include "Subjects/GameObject.h"

void ExplosionComponent::Update()
{
    if(m_SpriteComponent->m_SpriteInfo.m_CurrentCol != m_SpriteComponent->m_SpriteInfo.m_NrOfCols - 1) return;
    GetGameObjParent()->SetDestroyedFlag();
    m_SpriteComponent->m_IsActive = false;
}
