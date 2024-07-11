#include "ExplosionObserver.h"

#include "DataStructs.h"
#include "Initializers.h"
#include "Scene.h"
void ExplosionObserver::Notify(GameEngine::Subject* subject, int event,[[maybe_unused]] GameEngine::EventData* eventData)
{
    if (static_cast<GameEvent>(event) != GameEvent::died) return;
    auto explosion = InitExplosion(dynamic_cast<GameEngine::GameObject*>(subject));
    m_Scene->AddObject(std::move(explosion));
}
