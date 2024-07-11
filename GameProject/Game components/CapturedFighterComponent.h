#pragma once
#include <memory>

#include "Components/Component.h"
#include "Components/SpriteComponent.h"

class Trajectory;
class RotatingSprite;
class BossGalagaComponent;
class CapturedFighterComponent : public GameEngine::Component
{
public:
    explicit CapturedFighterComponent(GameEngine::GameObject* gameObj,BossGalagaComponent* parent,GameEngine::SpriteComponent* spriteComponent);
    void UploadGetBackTrajectory() const;
    void Update() override;
private:
    std::unique_ptr<RotatingSprite> m_RotatingSprite{};
    std::unique_ptr<Trajectory> m_GetBackTrajectory{nullptr};
    BossGalagaComponent* m_Parent{};
    const float m_Speed{200.f};
    
};
