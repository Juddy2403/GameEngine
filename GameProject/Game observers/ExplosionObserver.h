#pragma once
#include "IObserver.h"

namespace GameEngine
{
    class Scene;
}
class ExplosionObserver final : public GameEngine::IObserver
{
public:
    explicit ExplosionObserver(GameEngine::Scene* scene) : m_Scene(scene) {}
    void Notify(GameEngine::Subject* subject, int event, GameEngine::EventData* eventData) override;
private:
    GameEngine::Scene* m_Scene;
};
