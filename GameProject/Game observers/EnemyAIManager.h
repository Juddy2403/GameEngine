#pragma once
#include <vector>

#include "IObserver.h"
#include "Game components/Enemy components/EnemyComponent.h"

class EnemyAIManager final : public GameEngine::IObserver, public GameEngine::Component
{
public:
    explicit EnemyAIManager(GameEngine::GameObject* gameObj) : Component(gameObj) {}
    static void AddEnemy(EnemyComponent* enemy);
    static void RemoveEnemy(EnemyComponent* enemy);
    static void ShootBeam();
    static void BombingRun();
    void Notify(GameEngine::Subject* subject, int event, GameEngine::EventData* eventData) override;
    void Update() override;
private:
    int m_EnemiesInFormation{};
    static std::vector<EnemyComponent*> m_Enemies;
};
