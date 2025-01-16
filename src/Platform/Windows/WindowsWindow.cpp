#include "aepch.h"

#include "WindowsWindow.h"

#include <Core/Input.h>
#include <Core/KeyCodes.h>
#include <glad/glad.h>

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace AE::Platform::Windows
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        AE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Shutdown() const
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.title;
        m_Data.Width = props.width;
        m_Data.Height = props.height;

        AE_CORE_INFO("Initialising window...");

        if (!s_GLFWInitialized)
        {
            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialize GLFW");
            }
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        AE_CORE_INFO("GLFW initialised.");

        m_Window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);

        m_Context = new OpenGL::OpenGLContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Height = height;
            data.Width = width;

            Events::WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Events::WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
                {
                    Events::KeyPressedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    Events::KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    Events::KeyPressedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            default: break;
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, const unsigned int keycode)
        {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Events::KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
                {
                    Events::MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    Events::MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            default: break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset)
        {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Events::MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Events::MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });

        AE_CORE_INFO("Window initialised.");
    }

    void WindowsWindow::OnUpdate()
    {
        // Close window on command+w
        if (Input::IsKeyPressed(AE_KEY_LEFT_SUPER) && Input::IsKeyPressed(AE_KEY_W))
        {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(m_Window));
            Events::WindowCloseEvent e;
            data.EventCallback(e);
        }

        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(const bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const { return m_Data.VSync; }
}
