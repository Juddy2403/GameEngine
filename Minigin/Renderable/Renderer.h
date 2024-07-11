#pragma once
#include <SDL.h>
#include "../Managers/Singleton.h"

namespace GameEngine
{
    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
        SDL_Renderer* m_renderer{};
        SDL_Window* m_window{};
        SDL_Color m_clearColor{};
    public:
        void Init(SDL_Window* window);
        void Render() const;
        void Destroy();

        void RenderTexture(const Texture2D& texture, float x, float y) const;
        void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const SDL_Rect& destRect) const;
        void RenderRect(const SDL_Rect& rect, const SDL_Color& color) const;

        void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const SDL_Rect& destRect,
            float angle, SDL_Point center, const SDL_RendererFlip& flipMode) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const;

        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
        void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
    };
}
