#include "aepch.h"
#include "ImGuiLayer.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <Core/KeyCodes.h>

#include "GLFW/glfw3.h"

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/Application.h"

namespace AE::Graphics::UI
{
    static constexpr float DEFAULT_FONT_SIZE = 13.0f;
    static constexpr float RETINA_SCALE_FACTOR = 0.5f;

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
        if (!IMGUI_CHECKVERSION())
        {
            AE_CORE_ERROR("Failed to check ImGui!");
            return;
        }

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.IniFilename = nullptr;

        float xScale, yScale;
        unsigned int width, height;
        Core::Window::GetWindowProperties(&width, &height, &xScale, &yScale);

        // Configure font scaling for Retina displays
        io.FontGlobalScale = RETINA_SCALE_FACTOR;

        ImFontConfig fontConfig;
        fontConfig.SizePixels = DEFAULT_FONT_SIZE * xScale;
        io.Fonts->Clear();
        io.Fonts->AddFontDefault(&fontConfig);
        io.Fonts->SetTexID(static_cast<ImTextureID>(0));

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(xScale);

        auto* window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 150");

        io.Fonts->Build();
    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();

        float xScale, yScale;
        unsigned int width, height;
        Core::Window::GetWindowProperties(&width, &height, &xScale, &yScale);

        io.DisplaySize = ImVec2(width / xScale, height / yScale);
        io.DisplayFramebufferScale = ImVec2(xScale, yScale);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto const time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();

        // Ensure the viewport is consistent when dragging between monitors
        auto* window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ImGuiKey ImGuiLayer::MapSpecialKeys(const int keyCode)
    {
        switch (keyCode)
        {
        case AE_KEY_TAB: return ImGuiKey_Tab;
        case AE_KEY_LEFT: return ImGuiKey_LeftArrow;
        case AE_KEY_RIGHT: return ImGuiKey_RightArrow;
        case AE_KEY_UP: return ImGuiKey_UpArrow;
        case AE_KEY_DOWN: return ImGuiKey_DownArrow;
        case AE_KEY_ENTER: return ImGuiKey_Enter;
        case AE_KEY_ESCAPE: return ImGuiKey_Escape;
        case AE_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case AE_KEY_DELETE: return ImGuiKey_Delete;
        case AE_KEY_HOME: return ImGuiKey_Home;
        case AE_KEY_END: return ImGuiKey_End;
        case AE_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case AE_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        default: return ImGuiKey_None;
        }
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

    bool ImGuiLayer::OnMouseButtonPressedEvent(const Events::MouseButtonPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(const Events::MouseButtonReleasedEvent& event)
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

        io.AddKeyEvent(ImGuiMod_Ctrl, io.KeyCtrl);
        io.AddKeyEvent(ImGuiMod_Shift, io.KeyShift);
        io.AddKeyEvent(ImGuiMod_Alt, io.KeyAlt);
        io.AddKeyEvent(ImGuiMod_Super, io.KeySuper);

        const ImGuiKey imgui_key = MapSpecialKeys(event.GetKeyCode());
        if (imgui_key != ImGuiKey_None)
            io.AddKeyEvent(imgui_key, true);

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(const Events::KeyReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();

        const ImGuiKey imgui_key = MapSpecialKeys(event.GetKeyCode());
        if (imgui_key != ImGuiKey_None)
            io.AddKeyEvent(imgui_key, false);

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(const Events::KeyTypedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        const int typedChar = event.GetKeyCode();

        if (typedChar > 0 && typedChar < 0x10000)
            io.AddInputCharacter(static_cast<unsigned short>(typedChar));

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(const Events::WindowResizeEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();

        float xscale, yscale;
        unsigned int width, height;
        Core::Window::GetWindowProperties(&width, &height, &xscale, &yscale);

        io.DisplaySize = ImVec2(width / xscale, height / yscale);
        io.DisplayFramebufferScale = ImVec2(xscale, yscale);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }
}
