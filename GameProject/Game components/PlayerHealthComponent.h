#pragma once
#include "Components/Component.h"
#include "Components/SpriteComponent.h"

class PlayerHealthComponent final : public GameEngine::Component
{
public:
    explicit PlayerHealthComponent(GameEngine::GameObject* gameObject, int health,
        GameEngine::SpriteComponent* textureComponent);
    PlayerHealthComponent(const PlayerHealthComponent& other) = delete;
    PlayerHealthComponent(PlayerHealthComponent&& other) noexcept = delete;
    PlayerHealthComponent& operator=(const PlayerHealthComponent& other) = delete;
    PlayerHealthComponent& operator=(PlayerHealthComponent&& other) noexcept = delete;
    ~PlayerHealthComponent() override = default;

    void Render() override;
    void Hit();
    [[nodiscard]] int GetHealth() const { return m_Health; }
private:
    const glm::vec2 m_HealthPosition{ 10, 570 };
    const float m_TextureOffset{ 40 };
    GameEngine::SpriteComponent* m_TextureComponent{};
    int m_Health{};
};
