#pragma once

#include "Arnold/Graphics/Renderer/PerspectiveCamera.h"
#include "Arnold/Events/Event.h"
#include "Arnold/Events/MouseEvent.h"
#include "Arnold/Events/KeyEvent.h"
#include "Arnold/Events/ApplicationEvent.h"
#include "Arnold/Core/Timestep.h"

namespace AE::Graphics::Renderer
{
    /**
     * @class FpsCameraController
     * @brief Encapsulates all FPS-style camera input and movement logic.
     *
     * Owns a PerspectiveCamera and handles:
     * - WASD + Space/Shift movement (grounded XZ plane, vertical Y axis)
     * - Mouse look via ProcessMouseMovement
     * - ESC to toggle cursor capture
     * - Window resize to keep the aspect ratio correct
     *
     * Usage in a Layer:
     * @code
     * void OnUpdate(Timestep ts) override { m_Controller.OnUpdate(ts); }
     * void OnEvent(Event& e)     override { m_Controller.OnEvent(e); }
     * auto& cam = m_Controller.GetCamera();
     * @endcode
     */
    class FpsCameraController
    {
    public:
        FpsCameraController(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(Core::Timestep ts);
        void OnEvent(Events::Event& e);

        [[nodiscard]] PerspectiveCamera&       GetCamera()       { return m_Camera; }
        [[nodiscard]] const PerspectiveCamera& GetCamera() const { return m_Camera; }

        [[nodiscard]] bool  IsCursorCaptured() const { return m_CursorCaptured; }
        void SetMoveSpeed(float speed)               { m_MoveSpeed = speed; }
        [[nodiscard]] float GetMoveSpeed()     const { return m_MoveSpeed; }

    private:
        bool OnMouseMoved(Events::MouseMovedEvent& e);
        bool OnKeyPressed(Events::KeyPressedEvent& e);
        bool OnWindowResized(Events::WindowResizeEvent& e);

        PerspectiveCamera m_Camera;

        float m_MoveSpeed      = 5.0f;
        float m_LastMouseX     = 0.0f;
        float m_LastMouseY     = 0.0f;
        bool  m_FirstMouse     = true;
        bool  m_CursorCaptured = false;
    };
}
