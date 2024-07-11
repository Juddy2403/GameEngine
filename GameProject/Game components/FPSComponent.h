#pragma once
#include "Components/Component.h"
#include "Components/TextComponent.h"

class FPSComponent final : public GameEngine::Component
{
public:
    FPSComponent(GameEngine::GameObject* gameObj, GameEngine::TextComponent* textComponent) : Component(gameObj), m_TextComponent(textComponent) {}
    virtual void Update() override;
private:
    GameEngine::TextComponent* m_TextComponent;
    float m_FpsUpdateCounter{};
    int m_FramesSinceUpdate{};
    static const float m_FpsUpdateRate;
};
