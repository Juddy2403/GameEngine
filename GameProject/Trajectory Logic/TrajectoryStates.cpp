#include "TrajectoryStates.h"
#include "Game components/Enemy components/EnemyComponent.h"

void CircleTrajectory::Enter(PathData& pathData, const glm::vec2& currentPos)
{
    pathData.radius = glm::distance(currentPos, pathData.centerOfRotation);
    auto nextPos = TrajectoryMath::CalculateNextPositionInCircle(currentPos, pathData.centerOfRotation, pathData.radius, pathData.isRotatingClockwise);
    m_Direction = TrajectoryMath::CalculateDirection(currentPos, nextPos);
}
std::pair<glm::vec2, bool> CircleTrajectory::Update(PathData& pathData, float speed, const glm::vec2& currentPos)
{
    const auto nextPosInCircle = TrajectoryMath::CalculateNextPositionInCircle(currentPos, pathData.centerOfRotation, pathData.radius, pathData.isRotatingClockwise);
    m_Direction = TrajectoryMath::CalculateDirection(currentPos, nextPosInCircle);

    glm::vec2 nextPos = TrajectoryMath::CalculateNextPosition(currentPos, m_Direction, speed);
    const float currentAngle = std::atan2(-currentPos.y + pathData.centerOfRotation.y, currentPos.x - pathData.centerOfRotation.x);
    const float nextAngle = std::atan2(-nextPos.y + pathData.centerOfRotation.y, nextPos.x - pathData.centerOfRotation.x);
    float angleDifference = std::atan2(std::sin(nextAngle - currentAngle), std::cos(nextAngle - currentAngle));
    pathData.accumRotationAngle += std::abs(angleDifference);

    if (pathData.accumRotationAngle >= pathData.totalRotationAngle)
    {
        return { nextPos,false };
    }
    return { nextPos,true };
}
void LinearTrajectory::Enter(PathData& pathData, const glm::vec2& currentPos)
{
    m_Direction = TrajectoryMath::CalculateDirection(currentPos, pathData.destination);
}

std::pair<glm::vec2, bool> LinearTrajectory::Update(PathData& pathData, float speed, const glm::vec2& currentPos)
{
    glm::vec2 nextPos = TrajectoryMath::CalculateNextPosition(currentPos, m_Direction, speed);
    if (TrajectoryMath::ArePositionsEqual(pathData.destination, nextPos))
    {
        return { nextPos,false };
    }
    return { nextPos,true };
}
