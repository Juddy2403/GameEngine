#include "RotatingSprite.h"

#include <glm/trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "Components/SpriteComponent.h"


RotatingSprite::RotatingSprite(GameEngine::SpriteComponent* spriteComponent):
m_SpriteComponent(spriteComponent),
m_InitXPos{ spriteComponent->m_SpriteInfo.m_StartPos.x },
m_NrOfRotationStages((spriteComponent->m_SpriteInfo.m_NrOfCols - 1) * 4)
{
    InitColFlipPairs(spriteComponent->m_SpriteInfo.m_NrOfCols);
    spriteComponent->m_SpriteInfo.m_NrOfCols = 1;
}
void RotatingSprite::RotateSpriteInDirection(const glm::vec2& direction) const
{
    //calculate rotation angle based on the direction
    float rotationAngle = -(glm::atan(-direction.y, direction.x) - glm::pi<float>() / 2);
    //convert angle between 0 and 2*pi
    if (rotationAngle < 0) rotationAngle += glm::pi<float>() * 2;
    auto rotationStage = static_cast<int>(rotationAngle / (glm::pi<float>() * 2) * m_NrOfRotationStages);
    UpdateSprite(rotationStage);
}
void RotatingSprite::UpdateSprite(int rotationStage) const
{
    const auto rotationInfo = GetColFlipPair(rotationStage);
    m_SpriteComponent->m_SpriteInfo.m_StartPos.x = rotationInfo.first * (m_SpriteComponent->m_SpriteInfo.m_Width
        + m_SpriteComponent->m_SpriteInfo.m_Spacing) + m_InitXPos;
    m_SpriteComponent->SetFlipMode(rotationInfo.second);
    m_SpriteComponent->UpdateSrcRect();
}

void RotatingSprite::InitColFlipPairs(int nrOfCols)
{
    bool isColIncreasing = true;
    int col = 0;
    SDL_RendererFlip flipMode = SDL_FLIP_NONE;
    m_ColFlipPairs.emplace_back(col, SDL_FLIP_NONE);

    for (int i = 1; i < (nrOfCols - 1) * 4; ++i)
    {
        isColIncreasing ? ++col : --col;

        if (col == nrOfCols - 1)
        {
            isColIncreasing = false;
            flipMode = flipMode == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL;
        }
        else if (col == 0)
        {
            isColIncreasing = true;
            flipMode = flipMode == SDL_FLIP_HORIZONTAL ? SDL_FLIP_NONE : static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        }
        m_ColFlipPairs.emplace_back(col, flipMode);
    }
}
