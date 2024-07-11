#pragma once
#include <queue>
#include "Trajectory Logic/Trajectory.h"
#include "DataStructs.h"
#include "Components/Component.h"
#include "Enemy States/EnemyState.h"
#include "Game components/PlayerComponent.h"
#include "RotatingSprite.h"

namespace GameEngine
{
    class SpriteComponent;
}

class EnemyComponent : public GameEngine::Component
{
public:
    explicit EnemyComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent, PlayerComponent* playerComponent);

    EnemyComponent(const EnemyComponent& other) = delete;
    EnemyComponent(EnemyComponent&& other) noexcept = delete;
    EnemyComponent& operator=(const EnemyComponent& other) = delete;
    EnemyComponent& operator=(EnemyComponent&& other) noexcept = delete;
    ~EnemyComponent() override = default;

    virtual void GetInAttackState() = 0;
    virtual void GetInIdleState();
    void SetFormationPosition(const glm::ivec2& formationPos);
    void SetFormationTrajectory(const std::queue<PathData>& pathDataQueue);
    bool HasCurrentState() const { return m_CurrentState != nullptr; }
    bool HasSetOut() const;
    Trajectory& GetFormationTrajectory() { return m_FormationTrajectory; }
    
    [[nodiscard]] glm::ivec2 GetFormationPosition() const { return m_FormationPosition; }
    [[nodiscard]] float GetSpeed() const { return m_Speed; }
    virtual void Update() override;
    virtual bool HasBeenHit() {Died();return true;}
    virtual void Died();
    [[nodiscard]] virtual EnemyId GetEnemyID() const = 0;

    bool UpdateTrajectory(Trajectory& trajectory);
    glm::vec2 GetCurDirection() const { return m_CurDirection; }
    void SetCurDirection(const glm::vec2& direction) { m_CurDirection = direction; }
    PlayerComponent* GetPlayerComponent() const { return m_PlayerComponent; }
    RotatingSprite* GetRotatingSprite() const { return m_RotatingSprite.get(); }
    
    int m_SetOutTurn{};
    int m_Stage{};
protected:
    PlayerComponent* m_PlayerComponent{};
    std::unique_ptr<EnemyState> m_CurrentState;
    float m_Speed{ 300.f };
    std::unique_ptr<RotatingSprite> m_RotatingSprite{};
    glm::ivec2 m_FormationPosition{};
    float m_CurrentTime{};
    int m_CurrentRotationStage{};
private:
    //TODO: make trajectories unique ptrs 
    Trajectory m_FormationTrajectory{};
    glm::vec2 m_CurDirection{};
};


