#include "ScoreComponent.h"

#include "Game observers/ScoreManager.h"
ScoreComponent::ScoreComponent(GameEngine::GameObject* gameObj, GameEngine::TextComponent* textComp)
: Component(gameObj), m_TextComponent(textComp)
{
    m_LastScore = ScoreManager::GetPlayerScore();
    m_TextComponent->SetText(std::to_string(m_LastScore));
}

void ScoreComponent::Update()
{
    const auto currentScore = ScoreManager::GetPlayerScore(); 
    if(currentScore != m_LastScore)
    {
        m_LastScore = currentScore;
        m_TextComponent->SetText(std::to_string(m_LastScore));
    }
}
