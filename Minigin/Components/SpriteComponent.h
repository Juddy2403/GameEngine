#pragma once
#include <glm/vec2.hpp>
#include "TextureComponent.h"

namespace GameEngine
{
    struct SpriteInfo
    {
        glm::ivec2 m_StartPos{};
        int m_Width{};
        int m_Height{};
        int m_NrOfCols{};
        int m_NrOfRows{};
        int m_CurrentRow{};
        int m_CurrentCol{};
        int m_Spacing{};
        float m_TimeInterval{};

        SDL_Rect GetSrcRect()
        {
            SDL_Rect srcRect{};
            srcRect.x = m_StartPos.x + (m_Width + m_Spacing) * m_CurrentCol;
            srcRect.y = m_StartPos.y + (m_Height + m_Spacing) * m_CurrentRow;
            srcRect.w = m_Width;
            srcRect.h = m_Height;
            return srcRect;
        }
    };

    class SpriteComponent : public TextureComponent
    {
    public:
        explicit SpriteComponent(GameObject* gameObj);
        explicit SpriteComponent(GameObject* gameObj, const std::string& filename);
        explicit SpriteComponent(GameObject* gameObj, std::unique_ptr<Texture2D>&& texture);
        void UpdateSrcRect();
        virtual void Update() override;
        SpriteInfo m_SpriteInfo{};
        bool m_IsActive{ true };
        float m_Scale{};
    protected:
        float m_CurrentTimeElapsed{};
    };
}
