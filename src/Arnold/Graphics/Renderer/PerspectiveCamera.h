#pragma once

#include "Camera.h"
#include "glm/glm.hpp"

namespace AE::Graphics::Renderer
{
    /**
     * @class PerspectiveCamera
     * @brief FPS-style perspective camera using yaw and pitch angles
     *
     * Provides a perspective projection with first-person camera controls.
     * Movement direction vectors (forward, right) are exposed so that
     * callers can implement WASD movement relative to the camera's orientation.
     *
     * Usage:
     * - Call ProcessMouseMovement() each frame with the mouse delta
     * - Use GetForward() and GetRight() in the movement layer
     * - Call SetAspectRatio() on WindowResizeEvent
     */
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fovDegrees, float aspectRatio, float nearClip, float farClip);

        void SetPosition(const glm::vec3& position);
        void SetAspectRatio(float aspectRatio);

        /**
         * @brief Apply a mouse delta to yaw and pitch
         * @param xOffset Horizontal mouse delta (pixels)
         * @param yOffset Vertical mouse delta (pixels, positive = down)
         * @param sensitivity Degrees per pixel
         */
        void ProcessMouseMovement(float xOffset, float yOffset, float sensitivity = 0.1f);

        [[nodiscard]] glm::vec3 GetForward() const;
        [[nodiscard]] glm::vec3 GetRight() const;
        [[nodiscard]] glm::vec3 GetUp() const;

        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
        [[nodiscard]] float GetYaw()   const { return m_Yaw; }
        [[nodiscard]] float GetPitch() const { return m_Pitch; }

        [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }
        [[nodiscard]] const glm::mat4& GetViewMatrix()           const { return m_ViewMatrix; }
        [[nodiscard]] const glm::mat4& GetProjectionMatrix()     const { return m_ProjectionMatrix; }

    private:
        void RecalculateViewMatrix();
        void RecalculateProjectionMatrix();

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix{};

        glm::vec3 m_Position{ 0.0f, 0.0f, 3.0f };

        float m_Fov;
        float m_AspectRatio;
        float m_NearClip;
        float m_FarClip;

        float m_Yaw   = -90.0f;  // start facing -Z
        float m_Pitch =   0.0f;
    };
}
