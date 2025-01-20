#pragma once

#include "RenderCommand.h"

namespace AE::Graphics::Renderer
{
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
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);


        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
    };
}


