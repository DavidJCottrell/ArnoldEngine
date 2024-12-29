#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "GUI.h"

namespace AE
{
    class Window
    {
    public:
        Window();
        ~Window();

        GLFWwindow *window;

    private:
        std::unique_ptr<GUI> m_GUI;
        const char *getGlfwPlatformSpecifics();
    };
}
