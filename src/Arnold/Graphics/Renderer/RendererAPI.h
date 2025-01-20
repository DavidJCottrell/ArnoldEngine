#pragma once

#include "VertexArray.h"
#include "glm/glm.hpp"

namespace AE::Graphics::Renderer
{
    /**
     * @class RendererAPI
     * @brief Abstract base class defining the graphics API interface
     *
     * The RendererAPI provides a common interface for different graphics APIs
     * (like OpenGL, Vulkan, etc.). It defines the basic operations that any
     * graphics backend must implement:
     * - Clear color setting
     * - Screen clearing
     * - Indexed drawing
     *
     * The class uses a static API field to determine which concrete implementation
     * to use at runtime.
     */
    class RendererAPI
    {
    public:
        /**
         * @enum API
         * @brief Supported rendering APIs
         *
         * Enumerates the available graphics backends:
         * - None: No rendering (for headless operation)
         * - OpenGL: OpenGL rendering backend
         */
        enum class API
        {
            None = 0,
            OpenGL = 1,
        };

        virtual ~RendererAPI() = default;

        /**
        * @brief Sets the clear color for the render buffer
        * @param color RGBA color vector to use when clearing the screen
        */
        virtual void SetClearColor(const glm::vec4& color) = 0;

        /**
        * @brief Clears the render buffer using the current clear color
        */
        virtual void Clear() = 0;

        /**
         * @brief Performs indexed drawing using the provided vertex array
         * @param vertexArray Vertex array containing the geometry to draw
         */
        virtual void DrawIndexed(std::shared_ptr<Renderer::VertexArray> vertexArray) = 0;

        /**
         * @brief Gets the currently active rendering API
         * @return The active rendering API enumeration value
         */
        static API GetAPI() { return s_API; }

    private:
        static API s_API;
    };
}




