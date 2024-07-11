#pragma once
#include "EnemyState.h"

class GetInFormationState final : public EnemyState
{
public:
    virtual std::unique_ptr<EnemyState> Update(EnemyComponent* enemyComponent) override;
    virtual bool IsDiving() const override { return true; }
    virtual bool HasSetOut() const { return m_HasSetOut; }
private:
    bool m_HasSetOut{false};
    float m_AccumWaitTime{};
    float m_WaitTime{};
    static const float m_TimeInBetween;
};
