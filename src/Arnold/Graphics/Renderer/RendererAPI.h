#pragma once

#include "VertexArray.h"
#include "glm/glm.hpp"

namespace AE::Graphics::Renderer
{
    // Abstract. Implementation per platform
    class RendererAPI
    {
    public:
        /**
         * @enum API
         * @brief Enumeration of supported rendering APIs
         *
         * Defines the available graphics APIs that can be used by the engine:
         * - None: No rendering (for headless operation)
         * - OpenGL: OpenGL rendering backend
         * - Vulkan: Vulkan rendering backend (future)
         * - DirectX: DirectX rendering backend (future)
         */
        enum class API
        {
            None = 0,
            OpenGL = 1,
        };

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(std::shared_ptr<Renderer::VertexArray> vertexArray) = 0;

        static API GetAPI() { return s_API; }

    private:
        static API s_API;
    };
}




