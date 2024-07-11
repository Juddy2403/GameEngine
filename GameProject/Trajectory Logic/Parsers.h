#pragma once
#include <fstream>
#include <queue>
#include <json.hpp>

#include "Initializers.h"
#include "Minigin.h"
#include "PathDataStruct.h"
#include "Game components/Enemy components/EnemyComponent.h"
#include "Game observers/FormationObserver.h"

namespace Parser
{
    inline std::queue<PathData> ParseTrajectory(const std::string& filePath)
    {
        std::queue<PathData> pathDataQueue;
        std::ifstream fileStream(filePath);
        nlohmann::json jsonData;
        fileStream >> jsonData;

        for (const auto& element : jsonData)
        {
            PathData pathData{};
            pathData.isRotating = element["isRotating"].get<bool>();
            if (pathData.isRotating)
            {
                pathData.isRotatingClockwise = element["isRotatingClockwise"].get<bool>();
                pathData.totalRotationAngle = element["totalRotationAngle"].get<float>();
                pathData.centerOfRotation.x = element["centerOfRotation"][0].get<float>();
                pathData.centerOfRotation.y = element["centerOfRotation"][1].get<float>();
            }
            else
            {
                pathData.destination.x = element["destination"][0].get<float>();
                pathData.destination.y = element["destination"][1].get<float>();
            }
            pathDataQueue.push(pathData);
        }

        return pathDataQueue;
    }

    inline std::vector<std::vector<PathData>> ParseTrajectoryVec(const std::string& trajectoryPath, std::vector<glm::vec2>& startPositions)
    {
        std::vector<std::vector<PathData>> pathDataQueueVec;
        std::ifstream trajectoryFileStream(trajectoryPath);
        nlohmann::json trajectoryJsonData;
        trajectoryFileStream >> trajectoryJsonData;
        
        FormationObserver::SetNrOfStages(static_cast<int>(trajectoryJsonData.size()));
        for (const auto& element : trajectoryJsonData)
        {
            startPositions.emplace_back(element["startPos"][0].get<float>(), element["startPos"][1].get<float>());
            const auto trajectory = element["trajectory"];
            std::vector<PathData> pathDataQueue;
            for (const auto& path : trajectory)
            {
                PathData pathData{};
                pathData.isRotating = path["isRotating"].get<bool>();
                if (pathData.isRotating)
                {
                    pathData.isRotatingClockwise = path["isRotatingClockwise"].get<bool>();
                    pathData.totalRotationAngle = path["totalRotationAngle"].get<float>();
                    pathData.centerOfRotation.x = path["centerOfRotation"][0].get<float>();
                    pathData.centerOfRotation.y = path["centerOfRotation"][1].get<float>();
                }
                else
                {
                    if (path["destination"].is_string() && path["destination"].get<std::string>() == "formationPos")
                    {
                        pathData.destination = { -1,-1 };
                    }
                    else
                    {
                        pathData.destination.x = path["destination"][0].get<float>();
                        pathData.destination.y = path["destination"][1].get<float>();
                    }
                }
                pathDataQueue.emplace_back(pathData);
            }
            pathDataQueueVec.emplace_back(pathDataQueue);
        }

        return pathDataQueueVec;
    }

    inline std::vector<std::unique_ptr<GameEngine::GameObject>> ParseEnemyInfoByStage(const std::string& enemyInfoPath,
        const std::string& trajectoryPath, PlayerComponent* playerComponent)
    {
        std::vector<std::vector<PathData>> pathDataQueueVec;
        std::vector<glm::vec2> startPositions;
        pathDataQueueVec = ParseTrajectoryVec(trajectoryPath, startPositions);

        std::vector<std::unique_ptr<GameEngine::GameObject>> enemyVec;
        std::ifstream enemyFileStream(enemyInfoPath);
        nlohmann::json enemyJsonData;
        enemyFileStream >> enemyJsonData;

        for (const auto& element : enemyJsonData)
        {
            std::string enemyType = element["enemyType"];
            const auto positions = element["positions"];
            for (auto posElem : positions)
            {
                std::unique_ptr<GameEngine::GameObject> enemy{};
                glm::vec2 pos = { posElem["formationPosition"][0].get<float>(),posElem["formationPosition"][1].get<float>() };
                int formationStage = posElem["formationStage"];
                std::queue<PathData> pathDataQueue;
                auto pathDataVec = pathDataQueueVec[formationStage];
                for (auto path : pathDataVec)
                {
                    if (path.destination.x == -1 && path.destination.y == -1) path.destination = pos;
                    else if (posElem.contains("isXReversed") && posElem["isXReversed"].get<bool>())
                    {
                        constexpr int spriteOffset = 16;
                        path.destination.x = GameEngine::g_WindowRect.w - path.destination.x - spriteOffset;
                        path.centerOfRotation.x = GameEngine::g_WindowRect.w - path.centerOfRotation.x - spriteOffset;
                        path.isRotatingClockwise = !path.isRotatingClockwise;
                    }
                    pathDataQueue.push(path);
                }
                if (enemyType == "Bee") enemy = InitBee(playerComponent);
                else if (enemyType == "Butterfly") enemy = InitButterfly(playerComponent);
                else if (enemyType == "BossGalaga") enemy = InitBossGalaga(playerComponent);
                enemy->SetPosition({ startPositions[formationStage],0 });
                enemy->GetComponent<EnemyComponent>()->SetFormationPosition({ pos.x,pos.y });
                enemy->GetComponent<EnemyComponent>()->SetFormationTrajectory(pathDataQueue);
                int turn = posElem["turn"];
                enemy->GetComponent<EnemyComponent>()->m_SetOutTurn = turn;
                enemy->GetComponent<EnemyComponent>()->m_Stage = formationStage;
                enemyVec.emplace_back(std::move(enemy));
            }
        }
        return enemyVec;
    }
}
