#include "aepch.h"
#include "PerspectiveCamera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace AE::Graphics::Renderer
{
    PerspectiveCamera::PerspectiveCamera(const float fovDegrees, const float aspectRatio,
                                         const float nearClip, const float farClip)
        : m_Fov(fovDegrees), m_AspectRatio(aspectRatio),
          m_NearClip(nearClip), m_FarClip(farClip)
    {
        RecalculateProjectionMatrix();
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetAspectRatio(const float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
        RecalculateProjectionMatrix();
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::ProcessMouseMovement(const float xOffset, const float yOffset,
                                                  const float sensitivity)
    {
        m_Yaw   += xOffset * sensitivity;
        m_Pitch -= yOffset * sensitivity;  // subtract: mouse up -> look up

        if (m_Pitch >  89.0f) m_Pitch =  89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        RecalculateViewMatrix();
    }

    glm::vec3 PerspectiveCamera::GetForward() const
    {
        glm::vec3 forward;
        forward.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        forward.y = glm::sin(glm::radians(m_Pitch));
        forward.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        return glm::normalize(forward);
    }

    glm::vec3 PerspectiveCamera::GetRight() const
    {
        return glm::normalize(glm::cross(GetForward(), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    glm::vec3 PerspectiveCamera::GetUp() const
    {
        return glm::normalize(glm::cross(GetRight(), GetForward()));
    }

    void PerspectiveCamera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::perspective(
            glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void PerspectiveCamera::RecalculateViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_Position, m_Position + GetForward(), glm::vec3(0.0f, 1.0f, 0.0f));
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
