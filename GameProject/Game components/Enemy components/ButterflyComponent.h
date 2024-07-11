#pragma once
#include "EnemyComponent.h"

class ButterflyComponent final : public EnemyComponent
{
public:
    explicit ButterflyComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent, PlayerComponent* playerComponent);

    ButterflyComponent(const ButterflyComponent& other) = delete;
    ButterflyComponent(ButterflyComponent&& other) noexcept = delete;
    ButterflyComponent& operator=(const ButterflyComponent& other) = delete;
    ButterflyComponent& operator=(ButterflyComponent&& other) noexcept = delete;
    ~ButterflyComponent() override = default;

    virtual void GetInAttackState() override;
    virtual EnemyId GetEnemyID() const override;
private:
};

