#pragma once
#include <SDL_render.h>
#include <utility>
#include <vector>
#include <glm/vec2.hpp>

namespace GameEngine
{
    class SpriteComponent;
}
class RotatingSprite final
{
public:
    explicit RotatingSprite(GameEngine::SpriteComponent* spriteComponent);

    RotatingSprite(const RotatingSprite& other) = delete;
    RotatingSprite(RotatingSprite&& other) noexcept = delete;
    RotatingSprite& operator=(const RotatingSprite& other) = delete;
    RotatingSprite& operator=(RotatingSprite&& other) noexcept = delete;
    ~RotatingSprite() = default;

    int GetNrOfRotationStages() const { return m_NrOfRotationStages; }
    void RotateSpriteInDirection(const glm::vec2& direction) const;
    void UpdateSprite(int rotationStage) const;
private:
    std::pair<int,SDL_RendererFlip> GetColFlipPair(int index) const { return m_ColFlipPairs[index]; }
    GameEngine::SpriteComponent* m_SpriteComponent;
    int m_InitXPos{};
    int m_NrOfRotationStages;
    void InitColFlipPairs(int nrOfCols);
    std::vector<std::pair<int,SDL_RendererFlip>> m_ColFlipPairs;
};
