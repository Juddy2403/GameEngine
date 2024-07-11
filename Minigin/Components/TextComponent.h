#pragma once
#include <memory>
#include <SDL_pixels.h>
#include <string>

#include "Component.h"

namespace GameEngine
{
    class Font;
    class TextComponent final : public Component
    {
    public:
        explicit TextComponent(GameObject* gameObj, std::shared_ptr<Font> font = nullptr, const std::string& text = {}, const SDL_Color& color = { 255,255,255,255 });

        void SetText(const std::string& text);
        void SetFont(const std::shared_ptr<Font>& font);
        void SetColor(const SDL_Color& color);
        virtual void Update() override;
    private:
        SDL_Color m_Color;
        std::string m_Text{};
        std::shared_ptr<Font> m_Font{};
        bool m_NeedsUpdate{ true };
    };
}
