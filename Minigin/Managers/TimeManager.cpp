#include "TimeManager.h"

using namespace std::chrono;
using namespace GameEngine;

float TimeManager::m_ElapsedTime{};

float TimeManager::GetElapsed()
{
    return m_ElapsedTime;
}

high_resolution_clock::time_point TimeManager::GetCurrent() const
{
    return m_Current;
}

void TimeManager::Update()
{
    m_Current = high_resolution_clock::now();
    m_ElapsedTime = duration<float>(m_Current - m_Previous).count();
    if (m_ElapsedTime > m_ElapsedTimeCap) m_ElapsedTime = m_ElapsedTimeCap;
    m_Previous = m_Current;
}
