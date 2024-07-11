#include "EnemyAIManager.h"

#include "Galaga.h"
#include "Game components/Enemy components/BossGalagaComponent.h"

std::vector<EnemyComponent*> EnemyAIManager::m_Enemies;
void EnemyAIManager::AddEnemy(EnemyComponent* enemy)
{
    m_Enemies.emplace_back(enemy);
}
void EnemyAIManager::RemoveEnemy(EnemyComponent* enemy)
{
    std::erase(m_Enemies, enemy);
}
void EnemyAIManager::ShootBeam()
{
    auto it = std::ranges::find_if(m_Enemies,
                    [](EnemyComponent* enemy) { return enemy->GetEnemyID() == EnemyId::bossGalaga; });
    if (it != m_Enemies.end())
    {
        const auto bossGalagaComponent = dynamic_cast<BossGalagaComponent*>(*it);
        if(bossGalagaComponent->CanAttack()) bossGalagaComponent->GetInBeamAttackState();
    }
}
void EnemyAIManager::BombingRun()
{
    auto it = std::ranges::find_if(m_Enemies,
                [](EnemyComponent* enemy) { return enemy->GetEnemyID() == EnemyId::bossGalaga; });
    if (it != m_Enemies.end())
    {
        const auto bossGalagaComponent = dynamic_cast<BossGalagaComponent*>(*it);
        if(bossGalagaComponent->CanAttack()) (*it)->GetInAttackState();
    }
}
void EnemyAIManager::Notify([[maybe_unused]] GameEngine::Subject* subject, int event, [[maybe_unused]] GameEngine::EventData* eventData)
{
    switch (static_cast<GameEvent>(event))
    {
    case GameEvent::gotInFormation:
        ++m_EnemiesInFormation;
        break;
    case GameEvent::leftFormation:
        --m_EnemiesInFormation;
        break;
    default: break;
    }
}
void EnemyAIManager::Update()
{
    if (m_EnemiesInFormation != static_cast<int>(m_Enemies.size())) return;
    if (m_Enemies.empty())
    {
        Galaga::GetInstance().LevelCleared();
        return;
    }

    int enemyToSetOut = rand() % m_Enemies.size();
    if (m_Enemies[enemyToSetOut]->GetEnemyID() == EnemyId::bossGalaga)
    {
        auto boss = dynamic_cast<BossGalagaComponent*>(m_Enemies[enemyToSetOut]);
        if (rand() % 2 == 0 && !boss->HasCapturedFighter())
        {
            boss->GetInBeamAttackState();
            return;
        }
        boss->GetInAttackState();
        //set out butterfly
        auto it = std::ranges::find_if(m_Enemies,
            [](EnemyComponent* enemy) { return enemy->GetEnemyID() == EnemyId::butterfly; });
        if (it != m_Enemies.end()) (*it)->GetInAttackState();
        return;
    }
    m_Enemies[enemyToSetOut]->GetInAttackState();
    if (m_Enemies.size() == 1) return;
    //set out a second enemy
    if (rand() % 3 <= 1) m_Enemies[enemyToSetOut == 0 ? 1 : enemyToSetOut - 1]->GetInAttackState();
}
