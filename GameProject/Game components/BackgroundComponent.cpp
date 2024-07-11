#include "BackgroundComponent.h"

#include "Components/TextureComponent.h"
#include "Renderable/Renderer.h"
#include "Managers/TimeManager.h"
#include "Subjects/GameObject.h"

BackgroundComponent::BackgroundComponent(GameEngine::GameObject* gameObject, GameEngine::TextureComponent* textureComponent, int speed):
    Component(gameObject),
    m_TextureComponent(textureComponent),
    m_Speed(speed)
{}
void BackgroundComponent::Update()
{
    m_CurrentY += m_Speed * GameEngine::TimeManager::GetElapsed();
    if (m_CurrentY >= m_TextureComponent->m_DestRect.h)
    {
        m_CurrentY -= m_TextureComponent->m_DestRect.h;
    }
    GetGameObjParent()->SetPosition(0, m_CurrentY);
}
void BackgroundComponent::Render()
{
    SDL_Rect destRect = m_TextureComponent->m_DestRect;
    destRect.y = static_cast<int>(m_CurrentY) - destRect.h;
    //Rendering a second texture for the loop effect
    GameEngine::Renderer::GetInstance().RenderTexture(*m_TextureComponent->GetTexture(), m_TextureComponent->m_SrcRect, destRect);
}
