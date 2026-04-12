#pragma once

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"

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
        static void BeginScene(const Camera& camera);

        /**
         * @brief Ends the current scene
         *
         * Finalizes the current frame/scene and prepares for the next one.
         */
        static void EndScene();

        /**
        * @brief Submits a vertex array for rendering
        * @param shader The shader used to render the vertex array
        * @param vertexArray The vertex array to be rendered
        * @param transform The position in the world
        *
        * Queues the given geometry for rendering in the current scene.
        */
        static void Submit(const std::shared_ptr<Shader>& shader,
                           const std::shared_ptr<VertexArray>& vertexArray,
                           const glm::mat4& transform = glm::mat4(1.0f)
        );

        /**
         * @brief Gets the current graphics API
         * @return The active rendering API type
         */
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };
}


