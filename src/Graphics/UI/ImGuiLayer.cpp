#include "aepch.h"
#include "ImGuiLayer.h"
#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/Application.h"

namespace AE::Graphics::UI
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.IniFilename = nullptr;

        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void ImGuiLayer::OnUpdate()
    {
        using namespace AE::Core;

        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::Get();

        const float width = app.GetWindow().GetWidth();
        const float height = app.GetWindow().GetHeight();

        io.DisplaySize = ImVec2(width, height);
        io.DisplayFramebufferScale = ImVec2(2.0f, 2.0f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnDetach() {}

    void ImGuiLayer::OnEvent(AE::Events::Event &event)
    {
        AE::Events::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Events::MouseButtonPressedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<Events::MouseMovedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<Events::MouseScrolledEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<Events::KeyPressedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<Events::KeyReleasedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<Events::KeyTypedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<Events::WindowResizeEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(
        AE::Events::MouseButtonPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;

        return false; // Allow other layers to handle the event
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(
        AE::Events::MouseButtonReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(AE::Events::MouseMovedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(AE::Events::MouseScrolledEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheelH += event.GetXOffset();
        io.MouseWheel += event.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(AE::Events::KeyPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(AE::Events::KeyReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(AE::Events::KeyTypedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        int c = event.GetKeyCode();

        if (c > 0 && c < 0x10000)
            io.AddInputCharacter((unsigned short)c);

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(AE::Events::WindowResizeEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
        io.DisplayFramebufferScale = ImVec2(2.0f, 2.0f);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }
} // namespace AE::Graphics::UI