#pragma once
#include "Components/Component.h"
#include "Components/TextComponent.h"

class ScoreComponent final : public GameEngine::Component
{
public:
    explicit ScoreComponent(GameEngine::GameObject* gameObj,GameEngine::TextComponent* textComp);
    void Update() override;
private:
    GameEngine::TextComponent* m_TextComponent{};
    int m_LastScore{};
};
