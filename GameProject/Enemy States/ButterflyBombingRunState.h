#pragma once
#include "BombingRunState.h"

class ButterflyBombingRunState final : public BombingRunState
{
public:
    virtual void Enter([[maybe_unused]] EnemyComponent* enemyComponent) override;
};
