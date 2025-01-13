#include "aepch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Temp
#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
    }

    void ImGuiLayer::OnAttach()
    {
        if (!IMGUI_CHECKVERSION())
        {
            AE_CORE_ERROR("Failed to check ImGui!");
            return;
        }

        ImGui::CreateContext();
        if (!ImGui::GetCurrentContext())
        {
            AE_CORE_ERROR("Failed to create ImGui context!");
            return;
        }

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable multi-viewports

        // io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        // io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        float xScale, yScale;
        unsigned int width, height;
        Core::Window::GetWindowProperties(&width, &height, &xScale, &yScale);

        // Configure font scaling for Retina displays
        io.FontGlobalScale = RETINA_SCALE_FACTOR;

        ImFontConfig fontConfig;
        fontConfig.SizePixels = DEFAULT_FONT_SIZE * xScale;
        io.Fonts->Clear();
        io.Fonts->AddFontDefault(&fontConfig);
        io.Fonts->Build();
        io.Fonts->SetTexID(0);

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        style.ScaleAllSizes(xScale);
        ImGui::StyleColorsDark();

        auto* window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        AE_CORE_INFO("ImGuiLayer detached");
    }

    void ImGuiLayer::OnImGuiRender()
    {
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Core::Application& app = Core::Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_window = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_window);
        }

        // TODO: Test if this is needed
        // Ensure the viewport is consistent when dragging between monitors
        auto* window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }
}
