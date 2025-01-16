#pragma once
#include "Graphics/Renderer/Context.h"

struct GLFWwindow;

namespace AE::Platform::OpenGL
{
    class OpenGLContext final : public Graphics::Renderer::Context
    {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);
        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}

