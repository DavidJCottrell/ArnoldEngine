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
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
