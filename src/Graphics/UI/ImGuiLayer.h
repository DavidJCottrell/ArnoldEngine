#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace AE::Graphics::UI
{
    class ARNOLD_API ImGuiLayer : public AE::Core::Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(AE::Events::Event &event) override;

    private:
        // Mouse events
        bool OnMouseButtonPressedEvent(AE::Events::MouseButtonPressedEvent &event);
        bool OnMouseButtonReleasedEvent(AE::Events::MouseButtonReleasedEvent &event);
        bool OnMouseMovedEvent(AE::Events::MouseMovedEvent &event);
        bool OnMouseScrolledEvent(AE::Events::MouseScrolledEvent &event);

        // Keyboard events
        bool OnKeyPressedEvent(AE::Events::KeyPressedEvent &event);
        bool OnKeyReleasedEvent(AE::Events::KeyReleasedEvent &event);
        bool OnKeyTypedEvent(AE::Events::KeyTypedEvent &event);

        // Window events
        bool OnWindowResizeEvent(AE::Events::WindowResizeEvent &event);

    private:
        GLFWwindow *m_Window;
        float m_Time = 0.0f;
    };
}