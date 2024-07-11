#pragma once
#include "IObserver.h"

class EnemyObserver final : public GameEngine::IObserver
{
public:
    EnemyObserver() = default;
    virtual void Notify(GameEngine::Subject* subject, int event
        , GameEngine::EventData* eventData) override;
private:
};

