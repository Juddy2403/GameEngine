#include "Camera.h"
#include "TrsTransform.h"
#include "TimeManager.h"

void TrsTransform::Translate(const glm::vec3 &translation) {
    m_TranslationMatrix = glm::translate(m_TranslationMatrix, translation);
    m_NeedsUpdate = true;
}

void TrsTransform::Rotate(const glm::vec3 &rotation) {
    m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(rotation.x), Camera::right);
    m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(rotation.y), Camera::up);
    m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(rotation.z), Camera::forward);

    m_NeedsUpdate = true;
}

void TrsTransform::Scale(const glm::vec3 &scale) {
    m_ScaleMatrix = glm::scale(m_ScaleMatrix, scale);

    m_NeedsUpdate = true;
}

glm::mat4 TrsTransform::GetWorldMatrix() {
    if(m_NeedsUpdate) {
        m_WorldMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
        m_NeedsUpdate = false;
    }
    return m_WorldMatrix;
}

void TrsTransform::Update() {
    if(m_RotationPerSecond != glm::vec3(0.f)) {
        Rotate(m_RotationPerSecond* TimeManager::GetInstance().GetElapsed());
    }
}

void TrsTransform::SetRotationPerSecond(const glm::vec3 &rotationPerSecond) {
    m_RotationPerSecond = rotationPerSecond;
}
