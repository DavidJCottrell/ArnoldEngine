#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace AE
{
    class Window
    {
    public:
        Window();
        ~Window();

        GLFWwindow *window;

    private:
        const char *setGlfwPlatformSpecifics();
    };
}
