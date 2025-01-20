#pragma once
#include "Arnold/Graphics/Renderer/RendererAPI.h"

namespace AE::Platform::OpenGL
{
    class OpenGLRendererAPI : public Graphics::Renderer::RendererAPI
    {
    public:
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(std::shared_ptr<Graphics::Renderer::VertexArray> vertexArray) override;
    };
}

