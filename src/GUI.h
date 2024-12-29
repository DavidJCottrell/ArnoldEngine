#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace AE
{
    class GUI
    {
    public:
        GUI(GLFWwindow *window, const char *glsl_version);
        ~GUI();

        void Begin();
        void End();
        void OnImGuiRender();

    private:
        GLFWwindow *m_Window;
    };
}