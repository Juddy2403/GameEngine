#include "PlayerComponent.h"

#include "Galaga.h"
#include "GameCommands.h"
#include "PlayerHealthComponent.h"
#include "Enemy components/BossGalagaComponent.h"
#include "Managers/InputManager.h"

PlayerComponent::PlayerComponent(GameEngine::GameObject* gameObject, GameEngine::SpriteComponent* spriteComponent, int playerID):
    Component(gameObject),
    m_RotatingSprite(std::make_unique<RotatingSprite>(spriteComponent)),
    m_PlayerID(playerID)
{}

void PlayerComponent::GetCaptured(const glm::vec2& enemyPos)
{
    std::queue<PathData> pathDataQueue;
    PathData pathData{};

    pathData.destination = glm::vec2(enemyPos);
    pathData.destination.x += 30;
    pathDataQueue.push(pathData);

    // Set the trajectory
    m_CapturedTrajectory = std::make_unique<Trajectory>();
    m_CapturedTrajectory->SetPathData(pathDataQueue, GetGameObjParent()->GetPosition());

    m_IsGettingCaptured = true;
}

void PlayerComponent::BindCommands() const
{
    auto& input = GameEngine::InputManager::GetInstance();
    switch(Galaga::GetInstance().GetGameMode())
    {
    case GameMode::versus:
    {
        input.BindCommand(GameEngine::KeyboardInputKey::A,
       std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ -1.f,0.f }, m_PlayerSpeed));
        input.BindCommand(GameEngine::KeyboardInputKey::D,
            std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ 1.f,0.f }, m_PlayerSpeed));
        input.BindCommand(GameEngine::KeyboardInputKey::SPACE,
            std::make_unique<ShootBulletCommand>(GetGameObjParent()));
    }
        break;
    case GameMode::singlePlayer:
    {
        input.BindCommand(GameEngine::KeyboardInputKey::A,
       std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ -1.f,0.f }, m_PlayerSpeed));
        input.BindCommand(GameEngine::KeyboardInputKey::D,
            std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ 1.f,0.f }, m_PlayerSpeed));
        input.BindCommand(GameEngine::KeyboardInputKey::SPACE,
            std::make_unique<ShootBulletCommand>(GetGameObjParent()));
        //bind commands to controller
        input.BindCommand(GameEngine::ControllerInputKey::dpadLeft,
            std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ -1.f,0.f }, m_PlayerSpeed),0);
        input.BindCommand(GameEngine::ControllerInputKey::dpadRight,
            std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ 1.f,0.f }, m_PlayerSpeed),0);
        input.BindCommand(GameEngine::ControllerInputKey::X,
            std::make_unique<ShootBulletCommand>(GetGameObjParent()),0);
    }
        break;
    case GameMode::coop:
    {
        input.BindCommand(GameEngine::KeyboardInputKey::A,
       std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ -1.f,0.f }, m_PlayerSpeed));
        input.BindCommand(GameEngine::KeyboardInputKey::D,
            std::make_unique<GameEngine::Move>(GetGameObjParent(), glm::vec2{ 1.f,0.f }, m_PlayerSpeed));
        input.BindCommand(GameEngine::ControllerInputKey::X,
            std::make_unique<ShootBulletCommand>(GetGameObjParent()),0);
    }
        break;
    }
    
}
void PlayerComponent::Update()
{
    if (!m_IsGettingCaptured) return;
    
    if (m_CapturedTrajectory->IsComplete())
    {
        m_EnemyCapturing->GetGameObjParent()->NotifyAll(static_cast<int>(GameEvent::fighterCaptured));
        m_EnemyCapturing = nullptr;
        auto healthComp = GetGameObjParent()->GetComponent<PlayerHealthComponent>();
        GetGameObjParent()->SetPosition(m_RespawnPos);
        healthComp->Hit();
        BindCommands();
        m_IsGettingCaptured = false;
        m_RotatingSprite->RotateSpriteInDirection({0,-1});
        return;
    }
    const glm::vec2 currentPos = GetGameObjParent()->GetPosition();
    auto newPos = m_CapturedTrajectory->Update(m_Speed, currentPos).first;
    GetGameObjParent()->SetPosition({ newPos,0 });

    //rotate the sprite
    m_AccumTime += GameEngine::TimeManager::GetElapsed();
    if (m_AccumTime < m_TimeBetweenStages) return;
    ++m_CurrentRotationStage %= m_RotatingSprite->GetNrOfRotationStages();
    m_RotatingSprite->UpdateSprite(m_CurrentRotationStage);
    m_AccumTime -= m_TimeBetweenStages;
}
