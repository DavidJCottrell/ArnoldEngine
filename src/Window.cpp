#include "aepch.h"
#include "Window.h"

#include "Log.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace AE
{
    const char *Window::setGlfwPlatformSpecifics()
    {
#if defined(__APPLE__)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        return "#version 150";
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        return "#version 130";
#endif
    }

    void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            KeyPressedEvent event(key, 0);
            AE_INFO(event);
        }
        else
        {
            KeyReleasedEvent event(key);
            AE_INFO(event);
        }
    }

    void onMouseButton(GLFWwindow *window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                MouseButtonPressedEvent event(button);
                AE_INFO(event);
            }
            else
            {
                MouseButtonReleasedEvent event(button);
                AE_INFO(event);
            }
        }
    }

    Window::Window()
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        const char *glsl_version = setGlfwPlatformSpecifics();

        AE_CORE_INFO("GLFW Initialized. GLSL version: {0}", glsl_version);

        window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
        if (!window)
        {
            throw std::runtime_error("Failed to create GLFW window");
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("failed to initialize GLAD");
        }

        AE_CORE_INFO("OpenGL Initialized.");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);

        bool show_demo_window = true;
        bool show_another_window = false;

        AE_INFO("Application running...");
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            glfwSetKeyCallback(window, onKeyPress);
            glfwSetMouseButtonCallback(window, onMouseButton);

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
        }

        AE_CORE_INFO("Closing GLFW...");

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    Window::~Window() = default;
}
