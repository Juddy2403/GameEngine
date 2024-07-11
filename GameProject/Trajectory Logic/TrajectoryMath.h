#pragma once
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>

#include "Managers/TimeManager.h"

namespace TrajectoryMath
{
    inline glm::vec2 CalculateDirection(const glm::vec2& currentPos, const glm::vec2& destination)
    {
        return glm::normalize(destination - currentPos);
    }

    inline glm::vec2 CalculateNextPosition(const glm::vec2& currentPos, const glm::vec2& direction, float speed)
    {
        return currentPos + direction * speed * GameEngine::TimeManager::GetElapsed();
    }

    inline glm::vec2 CalculateNextPositionInCircle(const glm::vec2& currentPos, const glm::vec2& center, const float radius, bool isClockwise)
    {
        // Calculate the current angle of the currentPos with respect to the center
        float currentAngle = std::atan2(currentPos.y - center.y , currentPos.x - center.x);
        if (currentAngle < 0) currentAngle += glm::pi<float>() * 2;

        // Increment the angle to move clockwise
        float angleStep = glm::pi<float>() / 180;
        if(isClockwise) angleStep *= -1;
        const float nextAngle = currentAngle - angleStep; // Subtracting to move clockwise
        // Calculate the next position on the circle
        return {
            center.x + radius * std::cos(nextAngle),
            center.y + radius * std::sin(nextAngle)
        };
    }
    inline bool ArePositionsEqual(const glm::vec2& pos1, const glm::vec2& pos2)
    {
        return abs(pos1.x - pos2.x) <= 5 && abs(pos1.y - pos2.y) <= 5;
    }
};