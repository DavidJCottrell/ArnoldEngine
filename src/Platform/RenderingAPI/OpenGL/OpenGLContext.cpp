#include "aepch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace AE::Platform::OpenGL
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        if (m_WindowHandle == nullptr)
        {
            throw std::runtime_error("Window handle is null!");
        }
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        AE_CORE_INFO("GLAD Initialized.");
        AE_CORE_INFO("OpenGL Info:");
        AE_CORE_INFO("  OpenGL Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        AE_CORE_INFO("  OpenGL Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        AE_CORE_INFO("  OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
