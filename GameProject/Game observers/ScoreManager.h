#pragma once
#include <unordered_map>
enum class EnemyId;
class ScoreManager final
{
public:
    static int GetPlayerScore();
    static void AddScore(EnemyId enemyId);
    static int GetHighestScore();
private:
    static int m_PlayerScore;
    const static std::unordered_map<EnemyId, int> m_EnemyScores;
};
