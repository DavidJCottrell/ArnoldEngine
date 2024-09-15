#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cstdio>
#include <stdexcept>

namespace AE
{
    class Window
    {
    public:
        Window();
        ~Window();

        GLFWwindow *window;

    private:
        const char *getGLVersion();
    };
}
