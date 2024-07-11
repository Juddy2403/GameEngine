#pragma once
#include "Components/Component.h"

namespace GameEngine
{
    class SpriteComponent;
}
class ExplosionComponent final : public GameEngine::Component
{
public:
    explicit ExplosionComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent)
    : Component(gameObj), m_SpriteComponent(spriteComponent) {}
    void Update() override;
private:
    GameEngine::SpriteComponent* m_SpriteComponent;
};
