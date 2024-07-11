#include "Subject.h"
#include <ranges>
#include "Minigin/IObserver.h"

void GameEngine::Subject::AddObserver(int message, IObserver* observer)
{
    const auto it = m_Observers.find(message);
    if (it == m_Observers.end()) m_Observers[message] = ObserverList();
    m_Observers[message].push_front(observer);
    m_Observers[message].front()->Notify(this, -1);
}

void GameEngine::Subject::RemoveObserver(int message, IObserver* observer)
{
    if (m_Observers.contains(message)) m_Observers[message].remove(observer);
}

void GameEngine::Subject::NotifyAll(int event, EventData* eventData)
{
    for (const auto& val : m_Observers | std::views::values) {
        for (IObserver* observer : val) {
            observer->Notify(this, event, eventData);
        }
    }
}

void GameEngine::Subject::Notify(int event, int message, EventData* eventData)
{
    for (auto& observer : m_Observers[message])
        observer->Notify(this, event, eventData);
}
