#include "ScoreManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "DataStructs.h"

int ScoreManager::m_PlayerScore{};

const std::unordered_map<EnemyId, int> ScoreManager::m_EnemyScores{
    { EnemyId::bee,50 },
    { EnemyId::beeDiving,100 },
    { EnemyId::butterfly,80 },
    { EnemyId::butterflyDiving,160 },
    { EnemyId::bossGalaga,150 },
    { EnemyId::bossGalagaDiving,400 }
};

void ScoreManager::AddScore(EnemyId enemyId)
{
    m_PlayerScore += m_EnemyScores.at(enemyId);
#ifndef NDEBUG
    std::cout << "Player has score: " << m_PlayerScore << '\n';
#endif
}
int ScoreManager::GetHighestScore()
{
    std::ifstream inFile("../Data/HighestScores.txt");

    std::string line;
    std::getline(inFile, line);
    std::istringstream iss(line);
    std::string playerName;
    int playerScore;
    iss >> playerName >> playerScore;
    inFile.close();
    return playerScore;
}

int ScoreManager::GetPlayerScore()
{
    return m_PlayerScore;
}
