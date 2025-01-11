#pragma once

#include "imgui_impl_glfw.h"
#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace AE::Graphics::UI
{
    class ARNOLD_API ImGuiLayer : public Core::Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Events::Event& event) override;

        static ImGuiKey MapSpecialKeys(int keyCode);

    private:
        // Mouse events
        bool OnMouseButtonPressedEvent(const Events::MouseButtonPressedEvent& event);
        bool OnMouseButtonReleasedEvent(const Events::MouseButtonReleasedEvent& event);
        bool OnMouseMovedEvent(const Events::MouseMovedEvent& event);
        bool OnMouseScrolledEvent(const Events::MouseScrolledEvent& event);

        // Keyboard events
        bool OnKeyPressedEvent(const Events::KeyPressedEvent& event);
        bool OnKeyReleasedEvent(const Events::KeyReleasedEvent& event);
        bool OnKeyTypedEvent(const Events::KeyTypedEvent& event);

        // Window events
        bool OnWindowResizeEvent(const Events::WindowResizeEvent& event);

    private:
        GLFWwindow* m_Window{};
        float m_Time = 0.0f;
    };
}
