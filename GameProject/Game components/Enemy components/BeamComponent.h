#pragma once
#include "BossGalagaComponent.h"

class BeamComponent : public GameEngine::Component
{
public:
    explicit BeamComponent(GameEngine::GameObject* gameObj, GameEngine::SpriteComponent* spriteComponent, EnemyComponent* parentComp);
    void Update() override;
    EnemyComponent* GetParentComp() const { return m_ParentComp; }
    bool IsBeamRetracting() const { return m_IsBeamRetracting; }
    bool IsBeamActive() const;
private:
    bool m_IsBeamRetracting = false;
    int m_CurrentRow = 0;
    EnemyComponent* m_ParentComp;
    GameEngine::SpriteComponent* m_SpriteComponent;
};
