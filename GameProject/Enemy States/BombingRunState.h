#pragma once
#include "EnemyState.h"
#include "Trajectory Logic/Trajectory.h"

class BombingRunState: public EnemyState
{
public:
    virtual void Enter(EnemyComponent* enemyComponent) override;
    virtual std::unique_ptr<EnemyState> Update(EnemyComponent* enemyComponent) override;
    virtual bool IsDiving() const override { return true; }
protected:
    std::unique_ptr<Trajectory> m_BombingTrajectory{ std::make_unique<Trajectory>() };
private:
    bool m_NextBulletShot = false;
    const float m_TimeTillNextBulletShot = .6f;
    float m_AccumTime = 0.f;
};
