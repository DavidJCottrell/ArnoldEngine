#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Graphics/UI/GUI.h"

namespace AE::Core
{
    class Window
    {
    public:
        Window(const char *title);
        ~Window();

        GLFWwindow *window;

    private:
        std::unique_ptr<Graphics::UI::GUI> m_GUI;
        const char *getGlfwPlatformSpecifics();
    };
}
