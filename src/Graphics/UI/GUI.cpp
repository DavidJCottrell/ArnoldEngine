#include "aepch.h"
#include "GUI.h"
#include "Core/Log.h"
#include "Core/Core.h"
#include "Core/Window.h"

namespace AE::Graphics::UI
{
    GUI::GUI(GLFWwindow *window, const char *glsl_version)
        : m_Window(window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window, false);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    GUI::~GUI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUI::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUI::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUI::OnImGuiRender()
    {
        ImGui::Begin("Arnold Engine");

        if (ImGui::Button("Button"))
        {
            AE_INFO("Button clicked");
        }

        static char text[256] = "";
        if (ImGui::InputText("Input", text, sizeof(text)))
        {
            AE_INFO("Text changed to: {0}", text);
        }

        ImGui::End();
    }
}