#pragma once
#include <memory>

class EnemyComponent;

class EnemyState
{
public:
    virtual void Enter([[maybe_unused]] EnemyComponent* enemyComponent) {}
    virtual void Exit([[maybe_unused]] EnemyComponent* enemyComponent) {}
    virtual bool IsDiving() const = 0;
    virtual std::unique_ptr<EnemyState> Update(EnemyComponent* enemyComponent) = 0;
    virtual ~EnemyState() = default;
};


