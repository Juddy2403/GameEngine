#include "PlayerHealthComponent.h"

#include "DataStructs.h"
#include "Galaga.h"
#include "Renderable/Renderer.h"
#include "Subjects/GameObject.h"

PlayerHealthComponent::PlayerHealthComponent(GameEngine::GameObject* gameObject, int health,
    GameEngine::SpriteComponent* textureComponent):
    Component(gameObject),
    m_TextureComponent(textureComponent),
    m_Health(health)
{}
void PlayerHealthComponent::Render()
{
    for (int i{}; i < m_Health; ++i)
    {
        auto destRect = m_TextureComponent->m_DestRect;
        destRect.x = static_cast<int>(m_HealthPosition.x + i * m_TextureOffset);
        destRect.y = static_cast<int>(m_HealthPosition.y);
        auto srcRect = m_TextureComponent->m_SrcRect;
        srcRect.x = 1;
        GameEngine::Renderer::GetInstance().RenderTexture(*m_TextureComponent->GetTexture(), srcRect,
            destRect);
    }
}
void PlayerHealthComponent::Hit()
{
    if (m_Health <= 0) Galaga::GetInstance().GameLost(); 

    GetGameObjParent()->NotifyAll(static_cast<int>(GameEvent::died));

    --m_Health;
}
