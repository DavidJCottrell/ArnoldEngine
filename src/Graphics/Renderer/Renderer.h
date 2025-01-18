#pragma once

namespace AE::Graphics::Renderer
{
    /**
     * @enum RendererAPI
     * @brief Enumeration of supported rendering APIs
     *
     * Defines the available graphics APIs that can be used by the engine:
     * - None: No rendering (for headless operation)
     * - OpenGL: OpenGL rendering backend
     * - Vulkan: Vulkan rendering backend (future)
     * - DirectX: DirectX rendering backend (future)
     */
    enum class RendererAPI
    {
        None = 0,
        OpenGL = 1,
        Vulkan = 2,
        DirectX = 3,
    };

    /**
     * @class Renderer
     * @brief Core rendering system managing graphics API selection
     *
     * Provides a high-level interface for rendering operations and graphics API selection.
     * Centralizes rendering configuration and state management.
     *
     * Features:
     * - Graphics API selection (OpenGL, Vulkan, DirectX)
     * - Rendering state management
     * - Platform-agnostic rendering interface
     *
     * @note Currently supports OpenGL with architecture ready for additional APIs
     */
    class Renderer
    {
    public:
        static RendererAPI GetAPI() { return s_RendererAPI; };

    private:
        static RendererAPI s_RendererAPI;
    };
}


