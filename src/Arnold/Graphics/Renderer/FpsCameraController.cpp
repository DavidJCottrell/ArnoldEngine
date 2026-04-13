#include "aepch.h"
#include "FpsCameraController.h"

#include "Arnold/Core/Input.h"
#include "Arnold/Core/KeyCodes.h"
#include "Arnold/Core/Application.h"
#include "Arnold/Core/Window.h"

namespace AE::Graphics::Renderer
{
    FpsCameraController::FpsCameraController(float fov, float aspectRatio, float nearClip, float farClip)
        : m_Camera(fov, aspectRatio, nearClip, farClip)
    {
    }

    void FpsCameraController::OnUpdate(Core::Timestep ts)
    {
        if (!m_CursorCaptured)
            return;

        const glm::vec3 right      = m_Camera.GetRight();
        const glm::vec3 camForward = m_Camera.GetForward();

        // Project forward onto the XZ plane so movement stays grounded
        glm::vec3 moveForward(camForward.x, 0.0f, camForward.z);
        if (glm::length(moveForward) > 0.001f)
            moveForward = glm::normalize(moveForward);

        glm::vec3 moveDir(0.0f);
        if (Core::Input::IsKeyPressed(AE_KEY_W))          moveDir += moveForward;
        if (Core::Input::IsKeyPressed(AE_KEY_S))          moveDir -= moveForward;
        if (Core::Input::IsKeyPressed(AE_KEY_D))          moveDir += right;
        if (Core::Input::IsKeyPressed(AE_KEY_A))          moveDir -= right;
        if (Core::Input::IsKeyPressed(AE_KEY_SPACE))      moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
        if (Core::Input::IsKeyPressed(AE_KEY_LEFT_SHIFT)) moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);

        if (glm::length(moveDir) > 0.0f)
            moveDir = glm::normalize(moveDir);

        m_Camera.SetPosition(m_Camera.GetPosition() + moveDir * m_MoveSpeed * ts.GetSeconds());
    }

    void FpsCameraController::OnEvent(Events::Event& e)
    {
        Events::EventHandler handler(e);
        handler.TryHandle<Events::MouseMovedEvent>([this](Events::MouseMovedEvent& ev)   { return OnMouseMoved(ev);    });
        handler.TryHandle<Events::KeyPressedEvent>([this](Events::KeyPressedEvent& ev)   { return OnKeyPressed(ev);    });
        handler.TryHandle<Events::WindowResizeEvent>([this](Events::WindowResizeEvent& ev){ return OnWindowResized(ev); });
    }

    bool FpsCameraController::OnMouseMoved(Events::MouseMovedEvent& e)
    {
        if (!m_CursorCaptured)
            return false;

        if (m_FirstMouse)
        {
            m_LastMouseX = e.GetX();
            m_LastMouseY = e.GetY();
            m_FirstMouse = false;
            return false;
        }

        const float xOffset = e.GetX() - m_LastMouseX;
        const float yOffset = e.GetY() - m_LastMouseY;
        m_LastMouseX = e.GetX();
        m_LastMouseY = e.GetY();

        m_Camera.ProcessMouseMovement(xOffset, yOffset);
        return false;
    }

    bool FpsCameraController::OnKeyPressed(Events::KeyPressedEvent& e)
    {
        if (e.GetKeyCode() == AE_KEY_ESCAPE)
        {
            m_CursorCaptured = !m_CursorCaptured;
            m_FirstMouse     = true;
            Core::Application::Get().GetWindow().SetCursorMode(
                m_CursorCaptured ? Core::CursorMode::Captured : Core::CursorMode::Normal
            );
            return true;
        }
        return false;
    }

    bool FpsCameraController::OnWindowResized(Events::WindowResizeEvent& e)
    {
        if (e.GetWidth() > 0 && e.GetHeight() > 0)
            m_Camera.SetAspectRatio(static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight()));
        return false;
    }
}
