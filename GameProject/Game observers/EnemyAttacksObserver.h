#pragma once
#include "IObserver.h"
#include "Scene.h"

class EnemyAttacksObserver final : public GameEngine::IObserver
{
public:
    explicit EnemyAttacksObserver( GameEngine::Scene* scene);
    virtual void Notify(GameEngine::Subject* subject, int event
        , GameEngine::EventData* eventData) override;
private:
    GameEngine::Scene* m_Scene{nullptr};
};