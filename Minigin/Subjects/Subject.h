#pragma once
#include <forward_list>
#include <map>

namespace GameEngine
{
    struct EventData;
    enum class EngineGameEvent
    {
        collision
    };
    class IObserver;
    class Subject
    {
    private:
        typedef std::forward_list<IObserver*> ObserverList;
        typedef std::map<int, ObserverList> ObserversMap;

        ObserversMap m_Observers;
    public:
        void AddObserver(int message, IObserver* observer);
        void RemoveObserver(int message, IObserver* observer);
        void NotifyAll(int event, EventData* eventData = nullptr);
        void Notify(int event, int message, EventData* eventData = nullptr);

        Subject() = default;
        virtual ~Subject() = default;
        Subject(const Subject& other) = delete;
        Subject(Subject&& other) = delete;
        Subject& operator=(const Subject& other) = delete;
        Subject& operator=(Subject&& other) = delete;
    };
}
