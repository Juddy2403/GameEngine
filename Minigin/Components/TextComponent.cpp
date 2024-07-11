#include "TextComponent.h"
#include <SDL_ttf.h>
#include <stdexcept>

#include "TextureComponent.h"
#include "Minigin/Renderable/Font.h"
#include "Minigin/Renderable/Renderer.h"
#include "Minigin/Renderable/Texture2D.h"
#include "Minigin/Subjects/GameObject.h"

using namespace GameEngine;
TextComponent::TextComponent(GameObject* gameObj, std::shared_ptr<Font> font, const std::string& text, const SDL_Color& color) : Component(gameObj),
                                                                                                                                 m_Color(color)
{
    if (font) SetFont(font);
    SetText(text);
}
void TextComponent::SetText(const std::string& text)
{
    if (m_Text != text)
    {
        m_Text = text;
        m_NeedsUpdate = true;
    }
}

void TextComponent::SetFont(const std::shared_ptr<Font>& font)
{
    assert(font);
    if (font) m_Font = font;
}
void TextComponent::SetColor(const SDL_Color& color)
{
    m_Color = color;
    m_NeedsUpdate = true;
}

void TextComponent::Update()
{
    if (m_NeedsUpdate && GetGameObjParent()->CheckIfComponentExists<TextureComponent>())
    {
        const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
        if (surf == nullptr)
        {
            throw std::runtime_error(std::string("Render  text failed: ") + SDL_GetError());
        }
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (texture == nullptr)
        {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }
        SDL_FreeSurface(surf);
        GetGameObjParent()->GetComponent<TextureComponent>()->SetTexture(std::make_unique<Texture2D>(texture));
        m_NeedsUpdate = false;
    }
}
