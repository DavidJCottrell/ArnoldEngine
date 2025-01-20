#pragma once
#include "Graphics/Renderer/Context.h"

struct GLFWwindow;

namespace AE::Platform::OpenGL
{
    /**
     * @class OpenGLContext
     * @brief OpenGL implementation of the graphics context
     *
     * Handles OpenGL-specific initialization and context management:
     * - Creates and manages OpenGL context using GLFW
     * - Initializes GLAD for OpenGL function loading
     * - Handles buffer swapping for smooth rendering
     * - Provides OpenGL version and vendor information
     *
     * @note This class is specific to OpenGL and should not be used directly.
     * Use the generic Context interface instead.
     */
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

