#include "Trajectory.h"

std::pair<glm::vec2, bool> Trajectory::Update(float speed, const glm::vec2& currentPos)
{
    auto stateInfo = m_State->Update(m_PathData.front(), speed, currentPos);
    if (stateInfo.second == false)
    {
        m_PathData.pop();
        m_HasDirectionChanged = true;
        if (m_PathData.empty())
        {
            m_Direction = { 0,1 };
            m_IsComplete = true;
            return { stateInfo.first,m_HasDirectionChanged };
        }
        UpdateState(currentPos);
    }
    if(m_Direction != m_State->GetDirection())
    {
        m_Direction = m_State->GetDirection();
        m_HasDirectionChanged = true;
    }
    return { stateInfo.first,m_HasDirectionChanged };
 
}
void Trajectory::SetPathData(const std::queue<PathData>& pathData, const glm::vec2& currentPos)
{
    m_PathData = pathData;
    UpdateState(currentPos);
}

void Trajectory::UpdateState(const glm::vec2& currentPos) {
    if (m_PathData.front().isRotating) m_State = std::make_unique<CircleTrajectory>();
    else m_State = std::make_unique<LinearTrajectory>();
    m_State->Enter(m_PathData.front(),currentPos);
    m_Direction = m_State->GetDirection();
    m_HasDirectionChanged = true;
}
