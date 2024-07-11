#pragma once
#include <glm/vec2.hpp>

#include "PathDataStruct.h"
#include "TrajectoryMath.h"

class EnemyComponent;
class TrajectoryState
{
public:
    virtual ~TrajectoryState() = default;
    virtual void Enter([[maybe_unused]] PathData& pathData,[[maybe_unused]] const glm::vec2& currentPos) {}
    
    virtual std::pair<glm::vec2, bool> Update( PathData& pathData,float speed, const glm::vec2& currentPos) = 0;
    glm::vec2 GetDirection() const { return m_Direction; }
protected:
    glm::vec2 m_Direction{};
};

class CircleTrajectory : public TrajectoryState
{
public:
    virtual void Enter(PathData& pathData, const glm::vec2& currentPos) override;
    std::pair<glm::vec2, bool> Update( PathData& pathData,float speed, const glm::vec2& currentPos) override;
};

class LinearTrajectory : public TrajectoryState
{
    virtual void Enter(PathData& pathData, const glm::vec2& currentPos) override;
    std::pair<glm::vec2, bool> Update( PathData& pathData,float speed, const glm::vec2& currentPos) override;

};
