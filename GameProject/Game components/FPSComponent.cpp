#include "FPSComponent.h"
#include "Components/TextComponent.h"
#include "Managers/TimeManager.h"

const float FPSComponent::m_FpsUpdateRate = 1.f;
void FPSComponent::Update()
{
    m_FpsUpdateCounter += GameEngine::TimeManager::GetElapsed();
    ++m_FramesSinceUpdate;
    if (m_FpsUpdateCounter >= m_FpsUpdateRate)
    {
        const float fpsCounter{ m_FramesSinceUpdate / m_FpsUpdateCounter };
        std::stringstream stream{};
        stream << std::fixed << std::setprecision(1) << fpsCounter<<"FPS";
        m_TextComponent->SetText(stream.str());
        m_FramesSinceUpdate = 0;
        m_FpsUpdateCounter -= m_FpsUpdateRate;
    }
}
