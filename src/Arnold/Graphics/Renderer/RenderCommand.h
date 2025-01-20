#pragma once

#include "RendererAPI.h"

namespace AE::Graphics::Renderer
{
    /**
     * @class RenderCommand
     * @brief Static interface for rendering operations
     *
     * RenderCommand provides a simplified static interface to the rendering API.
     * It acts as a facade over the RendererAPI, making it easier to perform
     * common rendering operations without directly managing a RendererAPI instance.
     *
     * All methods are static and delegate to the underlying RendererAPI implementation.
     */
    class RenderCommand
    {
    public:
        /**
         * @brief Sets the clear color for the render buffer
         * @param color RGBA color vector to use when clearing the screen
         */
        static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        /**
         * @brief Clears the render buffer using the current clear color
         */
        static void Clear()
        {
            s_RendererAPI->Clear();
        }

        /**
         * @brief Performs indexed drawing using the provided vertex array
         * @param vertexArray Vertex array containing the geometry to draw
         */
        static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };
}
