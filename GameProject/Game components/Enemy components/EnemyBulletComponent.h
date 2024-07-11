#pragma once
#include <glm/vec2.hpp>

#include "Components/Component.h"

class EnemyBulletComponent final : public GameEngine::Component
{
public:
    EnemyBulletComponent(GameEngine::GameObject* gameObj,const glm::vec2& direction);
    void Update() override;
private:
    const float m_Speed{ 500.0f };
    glm::vec2 m_Direction{};
    
};
