#include "CapturedFighterComponent.h"

#include "FormationComponent.h"
#include "Enemy components/BossGalagaComponent.h"

CapturedFighterComponent::CapturedFighterComponent(GameEngine::GameObject* gameObj, BossGalagaComponent* parent, GameEngine::SpriteComponent* spriteComponent):
    Component(gameObj),
    m_RotatingSprite(std::make_unique<RotatingSprite>(spriteComponent)),
    m_GetBackTrajectory(std::make_unique<Trajectory>()),
    m_Parent(parent)
{}

void CapturedFighterComponent::UploadGetBackTrajectory() const
{
    std::queue<PathData> pathDataQueue;
    PathData pathData;

    // Initial movement to get behind the boss
    auto sprite = m_Parent->GetGameObjParent()->GetComponent<GameEngine::SpriteComponent>();
    pathData.destination = glm::vec2(0, -sprite->m_DestRect.h);
    pathDataQueue.push(pathData);
    auto localPos = GetGameObjParent()->GetLocalTransform().GetPosition();
    m_GetBackTrajectory->SetPathData(pathDataQueue, localPos);
}
void CapturedFighterComponent::Update()
{
    if (m_GetBackTrajectory)
    {
        if (m_GetBackTrajectory->IsComplete())
        {
            m_GetBackTrajectory = nullptr;
            return;
        }
        auto newPos = m_GetBackTrajectory->
        Update(m_Speed, GetGameObjParent()->GetLocalTransform().GetPosition()).first;
        
        m_RotatingSprite->RotateSpriteInDirection(m_GetBackTrajectory->GetDirection());
        GetGameObjParent()->SetPosition({ newPos,0 });
    }
    else
    {
        if (m_Parent->IsDiving()) m_RotatingSprite->RotateSpriteInDirection(m_Parent->GetCurDirection());
        else m_RotatingSprite->RotateSpriteInDirection({ FormationComponent::GetDirection(),0 });
    }
}
