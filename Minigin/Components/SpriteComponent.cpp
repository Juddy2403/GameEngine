#include "SpriteComponent.h"
#include "../Managers/TimeManager.h"

using namespace GameEngine;

SpriteComponent::SpriteComponent(GameObject* gameObj): TextureComponent(gameObj) {}
SpriteComponent::SpriteComponent(GameObject* gameObj, const std::string& filename):
    TextureComponent(gameObj, filename) {}
SpriteComponent::SpriteComponent(GameObject* gameObj, std::unique_ptr<Texture2D>&& texture) :
    TextureComponent(gameObj, std::move(texture)) {}

void SpriteComponent::UpdateSrcRect()
{
    m_SrcRect = m_SpriteInfo.GetSrcRect();
    if (m_SrcRect.w == 0 || m_SrcRect.h == 0)
    {
        m_DestRect.w = static_cast<int>(m_DestRect.w * m_Scale);
        m_DestRect.h = static_cast<int>(m_DestRect.h * m_Scale);
    }
    else
    {
        m_DestRect.w = static_cast<int>(m_SrcRect.w * m_Scale);
        m_DestRect.h = static_cast<int>(m_SrcRect.h * m_Scale);
    }
}
void SpriteComponent::Update()
{
    if (!m_IsActive) return;
    m_CurrentTimeElapsed += TimeManager::GetElapsed();
    if (m_CurrentTimeElapsed >= m_SpriteInfo.m_TimeInterval)
    {
        ++m_SpriteInfo.m_CurrentCol %= m_SpriteInfo.m_NrOfCols;
        if (m_SpriteInfo.m_CurrentCol == 0) ++m_SpriteInfo.m_CurrentRow %= m_SpriteInfo.m_NrOfRows;
        UpdateSrcRect();
        m_CurrentTimeElapsed -= m_SpriteInfo.m_TimeInterval;
    }
}