#include "aepch.h"
#include "Window.h"

#include "Log.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace AE::Core
{
    const char *Window::getGlfwPlatformSpecifics()
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
        // ImGui input handling
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        if (ImGui::GetIO().WantCaptureKeyboard)
            return;

        // Engine event handling
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            KeyPressedEvent event(key);
            AE_INFO(event);
        }
        else if (action == GLFW_RELEASE)
        {
            KeyReleasedEvent event(key);
            AE_INFO(event);
        }
    }

    void onMouseButton(GLFWwindow *window, int button, int action, int mods)
    {
        // ImGui input handling
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        if (ImGui::GetIO().WantCaptureMouse)
            return;

        // Engine event handling
        if (action == GLFW_PRESS)
        {
            MouseButtonPressedEvent event(button);
            AE_INFO(event);
        }
        else if (action == GLFW_RELEASE)
        {
            MouseButtonReleasedEvent event(button);
            AE_INFO(event);
        }
    }

    void onCharacterInput(GLFWwindow *window, unsigned int codepoint)
    {
        ImGui_ImplGlfw_CharCallback(window, codepoint);
    }

    Window::Window(const char *title)
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        const char *glsl_version = getGlfwPlatformSpecifics();

        AE_CORE_INFO("GLFW Initialized. GLSL version: {0}", glsl_version);

        window = glfwCreateWindow(640, 480, title, nullptr, nullptr);
        if (!window)
        {
            throw std::runtime_error("Failed to create GLFW window");
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("failed to initialize GLAD");
        }

        AE_CORE_INFO("OpenGL Initialized.");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);

        m_GUI = std::make_unique<AE::Graphics::UI::GUI>(window, glsl_version);

        // ---- Event Callbacks ----
        glfwSetKeyCallback(window, onKeyPress);
        glfwSetMouseButtonCallback(window, onMouseButton);
        glfwSetCharCallback(window, onCharacterInput);

        AE_INFO("Application running...");
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClear(GL_COLOR_BUFFER_BIT);

            m_GUI->Begin();
            m_GUI->OnImGuiRender();
            m_GUI->End();

            glfwSwapBuffers(window);
        }

        AE_CORE_INFO("Closing GLFW...");

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    Window::~Window() = default;
}
