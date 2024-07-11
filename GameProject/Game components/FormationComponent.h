#pragma once
#include "IObserver.h"
#include "Components/Component.h"

class FormationComponent final : public GameEngine::Component
{
public:
    explicit FormationComponent(GameEngine::GameObject* gameObj);
    void Update() override;
    ~FormationComponent() override = default;
    static float GetOffset();
    static void ToggleUpdate() { m_IsUpdating = !m_IsUpdating; }
    static int GetDirection() { return m_Direction; }
private:
    static bool m_IsUpdating;
    static float m_Offset;
    static int m_Direction;
    static constexpr int m_OffsetLimit{ 150 };
    static constexpr float m_Speed{ 60.f };
};
