#pragma once

namespace AE::Graphics::Renderer
{
    /**
     * @class Context
     * @brief Abstract interface for graphics context management
     *
     * Provides a common interface for managing graphics contexts across different
     * rendering backends and platforms.
     *
     * Responsibilities:
     * - Graphics context initialization
     * - Buffer swapping for double/triple buffering
     * - Graphics state management
     *
     * @note Specific implementations exist for different graphics APIs (e.g., OpenGL, Vulkan)
     */
    class Context
    {
    public:
        virtual ~Context() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}
