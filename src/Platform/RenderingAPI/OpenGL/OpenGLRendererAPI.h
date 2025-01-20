#pragma once
#include "Arnold/Graphics/Renderer/RendererAPI.h"

namespace AE::Platform::OpenGL
{
    /**
     * @class OpenGLRendererAPI
     * @brief OpenGL implementation of the RendererAPI interface
     *
     * Provides concrete OpenGL implementations for all rendering operations
     * defined in the RendererAPI interface. This class:
     * - Manages OpenGL state for clearing and drawing
     * - Handles OpenGL-specific buffer operations
     * - Implements indexed drawing using OpenGL vertex arrays
     */
    class OpenGLRendererAPI final : public Graphics::Renderer::RendererAPI
    {
    public:
        ~OpenGLRendererAPI() override = default;

        /**
         * @brief Sets the OpenGL clear color
         * @param color RGBA color vector used for glClearColor
         */
        void SetClearColor(const glm::vec4& color) override;

        /**
         * @brief Clears the OpenGL color and depth buffers
         */
        void Clear() override;

        /**
         * @brief Performs indexed drawing using OpenGL
         * @param vertexArray Vertex array containing the geometry to draw
         *
         * Uses glDrawElements to perform indexed rendering of the geometry
         * contained in the provided vertex array.
         */
        void DrawIndexed(std::shared_ptr<Graphics::Renderer::VertexArray> vertexArray) override;
    };
}

