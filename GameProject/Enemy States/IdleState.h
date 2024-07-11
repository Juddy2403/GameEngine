#pragma once
#include "EnemyState.h"
#include "Trajectory Logic/Trajectory.h"

class IdleState final : public EnemyState
{
public:
    void Enter([[maybe_unused]] EnemyComponent* enemyComponent) override;
    virtual std::unique_ptr<EnemyState> Update(EnemyComponent* enemyComponent) override;
    void Exit([[maybe_unused]] EnemyComponent* enemyComponent) override;
    virtual bool IsDiving() const override { return false; }
private:
    void UpdateBackToFormationTrajectory(EnemyComponent* enemyComponent) const;
    static void GotInFormation(EnemyComponent* enemyComponent);
    std::unique_ptr<Trajectory> m_BackToFormationTrajectory{ nullptr };
};
