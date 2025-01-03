#include "MacWindow.h"

#include <glad/glad.h>

#include "aepch.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace AE::Platform::Mac
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char *description)
    {
        AE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    MacWindow::MacWindow(const WindowProps &props)
    {
        MacWindow::Init(props);
    }

    MacWindow::~MacWindow()
    {
        Shutdown();
    }

    void MacWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void MacWindow::Init(const WindowProps &props)
    {
        AE_CORE_INFO("Initialising window...");

        m_Data.Title = props.title;
        m_Data.Width = props.width;
        m_Data.Height = props.height;

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

        AE_CORE_INFO("[1/2] - GLFW Initialized.");

        m_Window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        AE_CORE_INFO("[2/2] - GLAD Initialized.");

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
                                  {
                                    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                                    data.Height = height;
                                    data.Width = width;

                                    Events::WindowResizeEvent event(width, height);
                                    data.EventCallback(event); });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
                                   {
                                    const WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                                    Events::WindowCloseEvent event;
                                    data.EventCallback(event); });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
                                        const WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
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
                                        } });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, const unsigned int keycode)
                            {
                                            const WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                            Events::KeyTypedEvent event(keycode);
                                            data.EventCallback(event); });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
                                   {
                                            const WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
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
                                            } });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, const double xOffset, const double yOffset)
                              {
                                                const WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                                Events::MouseScrolledEvent event((float)xOffset, (float)yOffset);
                                                data.EventCallback(event); });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos)
                                 {
                                                    WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                                    Events::MouseMovedEvent event((float)xPos, (float)yPos);
                                                    data.EventCallback(event); });
    }

    void MacWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void MacWindow::SetVSync(const bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool MacWindow::IsVSync() const { return m_Data.VSync; }

}
