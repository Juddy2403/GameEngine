#pragma once
#include "Components/Component.h"

namespace GameEngine
{
    class TextureComponent;
}
class BackgroundComponent final : public GameEngine::Component
{
public:
    explicit BackgroundComponent(GameEngine::GameObject* gameObject, GameEngine::TextureComponent* textureComponent, int speed = 100);
    BackgroundComponent(const BackgroundComponent& other) = delete;
    BackgroundComponent(BackgroundComponent&& other) noexcept = delete;
    BackgroundComponent& operator=(const BackgroundComponent& other) = delete;
    BackgroundComponent& operator=(BackgroundComponent&& other) noexcept = delete;
    ~BackgroundComponent() override = default;

    void Update() override;
    void Render() override;
private:
    GameEngine::TextureComponent* m_TextureComponent;
    float m_CurrentY{ 0 };
    const int m_Speed{ 100 };
};
