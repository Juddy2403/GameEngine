#include "GameCommands.h"

#include "BulletTracker.h"
#include "DataStructs.h"
#include "Galaga.h"
#include "Subjects/GameObject.h"
#include "Game components/ModeSelectionComp.h"
#include "Game components/NameSelectionComp.h"
#include "Game observers/EnemyAIManager.h"

ShootBulletCommand::ShootBulletCommand(GameEngine::GameObject* actor): Command(actor) {}

void ShootBulletCommand::Execute()
{
    m_Current = GameEngine::TimeManager::GetInstance().GetCurrent();
    const std::chrono::duration<float> elapsed = m_Current - m_Previous;

    if (m_ShootsAvailable < m_MaxShootCount && elapsed.count() > m_ShootCooldown) m_ShootsAvailable = m_MaxShootCount;
    if (m_ShootsAvailable <= 0) return;

    --m_ShootsAvailable;
    m_Actor->Notify(static_cast<int>(GameEvent::bulletShot),
        static_cast<int>(ObserverIdentifier::bullet));

    m_Previous = m_Current;
    BulletTracker::BulletFired();
}

GameEngine::Command::ExecuteOn ShootBulletCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
SwitchModesCommand::SwitchModesCommand(GameEngine::GameObject* actor, bool movingUp):
    Command(actor), m_ModeSelectionComp(actor->GetComponent<ModeSelectionComp>()),
    m_IsMovingUp(movingUp)
{}
void SwitchModesCommand::Execute()
{
    if (m_IsMovingUp) m_ModeSelectionComp->MoveUp();
    else m_ModeSelectionComp->MoveDown();
}
GameEngine::Command::ExecuteOn SwitchModesCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}

SelectModeCommand::SelectModeCommand(GameEngine::GameObject* actor):
    Command(actor),
    m_ModeSelectionComp(actor->GetComponent<ModeSelectionComp>()) {}
void SelectModeCommand::Execute()
{
    GameMode mode = m_ModeSelectionComp->GetSelectedMode();
    Galaga::GetInstance().SetGameMode(mode);
}
GameEngine::Command::ExecuteOn SelectModeCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
ShootBeamCommand::ShootBeamCommand(GameEngine::GameObject* actor): Command(actor) {}
void ShootBeamCommand::Execute()
{
    EnemyAIManager::ShootBeam();
}
GameEngine::Command::ExecuteOn ShootBeamCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
BombingRunCommand::BombingRunCommand(GameEngine::GameObject* actor): Command(actor) {}
void BombingRunCommand::Execute()
{
    EnemyAIManager::BombingRun();
}
GameEngine::Command::ExecuteOn BombingRunCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
MuteCommand::MuteCommand(GameEngine::GameObject* actor): Command(actor) {}
void MuteCommand::Execute()
{
    if(Galaga::volume == 0) Galaga::volume = Galaga::baseVolume;
    else Galaga::volume = 0;
}
GameEngine::Command::ExecuteOn MuteCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
SwitchNameCommand::SwitchNameCommand(GameEngine::GameObject* actor, bool movingUp):
Command(actor), m_NameSelectionComp(actor->GetComponent<NameSelectionComp>()),m_IsMovingUp(movingUp){}
void SwitchNameCommand::Execute()
{
    if(m_IsMovingUp) m_NameSelectionComp->MoveUp();
    else m_NameSelectionComp->MoveDown();
}
GameEngine::Command::ExecuteOn SwitchNameCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
SelectNameCommand::SelectNameCommand(GameEngine::GameObject* actor):
Command(actor), m_NameSelectionComp(actor->GetComponent<NameSelectionComp>()){}
void SelectNameCommand::Execute()
{
    if(m_NameSelectionComp->MoveRight()) Galaga::GetInstance().SetPlayerName(m_NameSelectionComp->GetName());
}
GameEngine::Command::ExecuteOn SelectNameCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
LoadChooseNameCommand::LoadChooseNameCommand(GameEngine::GameObject* actor): Command(actor) {}
void LoadChooseNameCommand::Execute()
{
    Galaga::GetInstance().ChooseName();
}
GameEngine::Command::ExecuteOn LoadChooseNameCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
SkipLevelCommand::SkipLevelCommand(GameEngine::GameObject* actor): Command(actor) {}
void SkipLevelCommand::Execute()
{
    Galaga::GetInstance().LevelCleared();
}
GameEngine::Command::ExecuteOn SkipLevelCommand::ExecuteOnKeyState() const
{
    return ExecuteOn::keyDown;
}
