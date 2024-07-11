#pragma once
#include <memory>
#include <SDL_render.h>
#include <string>

#include "Component.h"

namespace GameEngine
{
    class Texture2D;
    class TextureComponent : public Component
    {
    public:
        explicit TextureComponent(GameObject* gameObj);
        explicit TextureComponent(GameObject* gameObj, const std::string& filename);
        explicit TextureComponent(GameObject* gameObj, std::unique_ptr<Texture2D>&& texture);
        //virtual void Update() override;
        virtual void Render() override;
        [[nodiscard]] Texture2D* GetTexture() const;
        void SetTexture(const std::string& filename);
        void SetTexture(std::unique_ptr<Texture2D>&& texture);

        SDL_Rect m_DestRect{};
        SDL_Rect m_SrcRect{};

        void SetFlipMode(const SDL_RendererFlip& flipMode) { m_FlipMode = flipMode; }
        void SetRotationAngle(float angle) { m_RotationAngle = angle; }
        void SetRotationCenter(const SDL_Point& center) { m_RotationCenter = center; }
    protected:
        float m_RotationAngle{};
        SDL_Point m_RotationCenter{};
        SDL_RendererFlip m_FlipMode{ SDL_FLIP_NONE };
        void InitRects();
        Texture2D* m_Texture{};
    };
}

