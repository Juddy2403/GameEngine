#include "Command.h"

#include "Minigin/Minigin.h"
#include "Minigin/Subjects/GameObject.h"
#include "Minigin/Managers/TimeManager.h"

using namespace GameEngine;

Command::Command(GameObject* actor) : m_Actor{ actor } {}

Move::Move(GameObject* actor, const glm::vec2& direction, int speed):
    Command(actor),
    m_Direction(direction),
    m_Speed(speed)
{}

Move::~Move() {}

void Move::Execute()
{
    //check if the actor is hitting borders
    if (m_Actor->GetLocalTransform().GetPosition().x <= 0 && m_Direction.x < 0) return;
    if (m_Actor->GetLocalTransform().GetPosition().x >= g_WindowRect.w - 32 && m_Direction.x > 0) return;
    if (m_Actor->GetLocalTransform().GetPosition().y <= 0 && m_Direction.y < 0) return;
    if (m_Actor->GetLocalTransform().GetPosition().y >= g_WindowRect.h && m_Direction.y > 0) return;
    m_Actor->GetLocalTransform().Translate(TimeManager::GetElapsed() * m_Speed * m_Direction);
}
Command::ExecuteOn Move::ExecuteOnKeyState() const { return ExecuteOn::keyPressed; }


