#pragma once
#include "IObserver.h"

class FormationObserver : public GameEngine::IObserver
{
public:
    void Notify(GameEngine::Subject* subject, int event, GameEngine::EventData* eventData = nullptr) override;
    static int GetCurrentStage() { return m_CurrentStage; }
    static void EnemySetOut() { ++m_CurrentEnemiesSetOut; }
    static void SetNrOfStages(int nrOfStages) { m_NrOfStages = nrOfStages; }
private:
    static int m_NrOfStages;
    static int m_CurrentStage;
    static int m_CurrentEnemiesSetOut;
    static int m_CurrentEnemiesGotInFormation;
};
