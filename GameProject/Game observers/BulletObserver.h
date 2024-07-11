#pragma once
#include "IObserver.h"
#include "Scene.h"

class BulletObserver final : public GameEngine::IObserver
{
public:
    explicit BulletObserver( GameEngine::Scene* scene);
    virtual void Notify(GameEngine::Subject* subject, int event
        , GameEngine::EventData* eventData) override;
private:
    GameEngine::Scene* m_Scene{nullptr};
};