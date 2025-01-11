#include "aepch.h"
#include "ImGuiLayer.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

#include "GLFW/glfw3.h"

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/Application.h"


namespace AE::Graphics::UI
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
        if (ImGui::GetCurrentContext() != nullptr)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.IniFilename = nullptr;

        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();

        float xScale, yScale;
        unsigned int width, height;
        GetWindowProperties(&width, &height, &xScale, &yScale);

        io.DisplaySize = ImVec2(width, height);
        io.DisplayFramebufferScale = ImVec2(xScale, yScale);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        auto const time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::GetWindowProperties(unsigned int* width, unsigned int* height,
        float* xScale, float* yScale)
    {
        using namespace AE::Core;
        const Application& app = Application::Get();

        *width = app.GetWindow().GetWidth();
        *height = app.GetWindow().GetHeight();

        auto* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        glfwGetWindowContentScale(window, xScale, yScale);
    }

    void ImGuiLayer::OnDetach()
    {
        AE_CORE_INFO("ImGuiLayer detached");
    }

    void ImGuiLayer::OnEvent(Events::Event& event)
    {
        Events::EventHandler handler(event);
        handler.TryHandle<Events::MouseButtonPressedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        handler.TryHandle<Events::MouseButtonReleasedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        handler.TryHandle<Events::MouseMovedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        handler.TryHandle<Events::MouseScrolledEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        handler.TryHandle<Events::KeyPressedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        handler.TryHandle<Events::KeyReleasedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        handler.TryHandle<Events::KeyTypedEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        handler.TryHandle<Events::WindowResizeEvent>(AE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(
        const Events::MouseButtonPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;

        return false; // Allow other layers to handle the event
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(
        const Events::MouseButtonReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(const Events::MouseMovedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(const Events::MouseScrolledEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += event.GetXOffset();
        io.MouseWheel += event.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(const Events::KeyPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(const Events::KeyReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(const Events::KeyTypedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        int c = event.GetKeyCode();

        if (c > 0 && c < 0x10000)
            io.AddInputCharacter((unsigned short)c);

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(const Events::WindowResizeEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();

        float xscale, yscale;
        unsigned int width, height;
        GetWindowProperties(&width, &height, &xscale, &yscale);

        io.DisplaySize = ImVec2(width, height);
        io.DisplayFramebufferScale = ImVec2(xscale, yscale);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }
} // namespace AE::Graphics::UI
