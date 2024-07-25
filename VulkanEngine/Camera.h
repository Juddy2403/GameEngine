#pragma once
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace VulkanEngine
{
    constexpr float g_Near = 0.1f;
    constexpr float g_Far = 1000.f;
    struct Camera
    {
        Camera() = default;

        glm::vec3 m_Origin{};
        float m_FovAngle{ 90.f };

        constexpr static glm::vec3 up{ 0.f,1.f,0.f };
        constexpr static glm::vec3 forward{ 0.f,0.f,1.f };
        constexpr static glm::vec3 right{ 1.f,0.f,0.f };

        glm::vec3 m_Forward{ forward };
        glm::vec3 m_Up{ up };
        glm::vec3 m_Right{ right };

        float m_TotalPitch{};
        float m_TotalYaw{};

        glm::mat4x4 m_ViewMatrix{};
        glm::mat4x4 m_ProjectionMatrix{};

        float m_RotationSpeed{ 20.f };
        float m_MovementSpeed{ 10.f };

        void Initialize(const float fovAngle = 90.f, const glm::vec3& origin = { 0.f,0.f,0.f }, const float aspectRatio = 1.f)
        {
            m_FovAngle = glm::radians(fovAngle);
            CalculateProjectionMatrix(aspectRatio);
            m_Origin = origin;
        }

        void CalculateViewMatrix()
        {
            m_ViewMatrix = glm::lookAtLH(m_Origin, m_Origin + m_Forward, m_Up);
        }

        void CalculateProjectionMatrix(const float aspectRatio)
        {
            m_ProjectionMatrix = glm::perspectiveLH(m_FovAngle, aspectRatio, g_Near, g_Far);
            m_ProjectionMatrix[1][1] *= -1;
        }

        void Update()
        {
            glm::mat4x4 finalRotation = glm::rotate(glm::mat4x4(1.f), glm::radians(m_TotalPitch), m_Right);
            finalRotation = glm::rotate(finalRotation, glm::radians(m_TotalYaw), up);

            m_Forward = glm::vec3(glm::normalize(finalRotation * glm::vec4{ 0,0,1,0 }));
            m_Right = glm::normalize(glm::cross({ 0,1,0 }, m_Forward));
            m_Up = glm::normalize(glm::cross(m_Forward, m_Right));
        
            //Update Matrices
            CalculateViewMatrix();
        }
    };
}
