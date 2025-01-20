#pragma once

#include "RenderCommand.h"

namespace AE::Graphics::Renderer
{
    /**
     * @class Renderer
     * @brief High-level rendering system manager
     *
     * The Renderer class manages the high-level rendering pipeline. It:
     * - Controls scene rendering state through BeginScene/EndScene
     * - Submits drawable objects to the rendering queue
     * - Provides access to the current graphics API
     *
     * This class works in conjunction with RenderCommand to provide both
     * high-level scene management and low-level rendering operations.
     */
    class Renderer
    {
    public:
        /**
         * @brief Begins a new scene for rendering
         *
         * Sets up any necessary state for beginning a new frame/scene.
         */
        static void BeginScene();

        /**
         * @brief Ends the current scene
         *
         * Finalizes the current frame/scene and prepares for the next one.
         */
        static void EndScene();

        /**
        * @brief Submits a vertex array for rendering
        * @param vertexArray The vertex array to be rendered
        *
        * Queues the given geometry for rendering in the current scene.
        */
        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        /**
         * @brief Gets the current graphics API
         * @return The active rendering API type
         */
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
    };
}


