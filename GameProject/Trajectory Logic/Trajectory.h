#pragma once
#include <memory>
#include <queue>
#include <glm/vec2.hpp>

#include "PathDataStruct.h"
#include "TrajectoryStates.h"


class EnemyComponent;
class Trajectory
{
public:
    std::pair<glm::vec2,bool> Update(float speed, const glm::vec2& currentPos);
    void SetPathData(const std::queue<PathData>& pathData, const glm::vec2& currentPos);
    ~Trajectory() = default;
    [[nodiscard]] glm::vec2 GetDirection() const { return m_Direction; }
    [[nodiscard]] bool IsComplete() const { return m_IsComplete; }
private:
    bool m_HasDirectionChanged = false;
    bool m_IsComplete = false;
    void UpdateState(const glm::vec2& currentPos);
    glm::vec2 m_Direction{};
    std::queue<PathData> m_PathData;
    std::unique_ptr<TrajectoryState> m_State;
};
