#pragma once

class EnemyComponent;
namespace GameEngine
{
    class GameObject;
}
// ┌────────────────────────┐                 ┌────────────────────────┐  
// │                        │ has been hit    │                        │  
// │        Stage 1         ├───────────────► │        Stage 2         ├─┐
// │                        │                 │                        │ │
// └────────────────────────┘                 ├────────────────────────┤ │
//                                            │change sprites          │ │
//                                            └────────────────────────┘ │
//                                                                       │
//                                                                       │
//                         ┌────────────────┐   has been hit             │
//                         │      Dies      │ ◄──────────────────────────┘
//                         ├────────────────┤                             
//                         │ score increases│                             
//                         └────────────────┘                             
class BossHealthStage
{
public:
    virtual BossHealthStage* HasBeenHit(EnemyComponent* bossObj) = 0;
    virtual ~BossHealthStage() = default;
};

class BossStageOne final : public BossHealthStage
{
public:
    virtual BossHealthStage* HasBeenHit(EnemyComponent* bossObj) override;
    virtual ~BossStageOne() = default;
};

class BossStageTwo final : public BossHealthStage
{
public:
    virtual BossHealthStage* HasBeenHit(EnemyComponent* bossObj) override;
    virtual ~BossStageTwo() = default;
};
