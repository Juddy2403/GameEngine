#pragma once
#include <glm/vec2.hpp>

struct PathData
{
    bool isRotating{false};
    bool isRotatingClockwise{true};
    float radius{};
    float totalRotationAngle{};
    float accumRotationAngle{};
    glm::vec2 centerOfRotation{};
    glm::vec2 destination{};
};
