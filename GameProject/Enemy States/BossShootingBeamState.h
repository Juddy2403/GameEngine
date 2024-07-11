#pragma once
#include "EnemyState.h"
#include "Trajectory Logic/Trajectory.h"

class BossShootingBeamState final : public EnemyState
{
public:
    virtual void Enter([[maybe_unused]] EnemyComponent* enemyComponent) override;
    virtual std::unique_ptr<EnemyState> Update(EnemyComponent* enemyComponent) override;
    virtual bool IsDiving() const override { return true; }
private:
    bool m_IsShootingBeam = false;
    std::unique_ptr<Trajectory> m_BeamTrajectory{ std::make_unique<Trajectory>() };
};