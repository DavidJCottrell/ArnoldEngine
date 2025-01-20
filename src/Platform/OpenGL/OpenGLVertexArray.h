#pragma once
#include <Graphics/Renderer/Buffer.h>

#include "Graphics/Renderer/VertexArray.h"

namespace AE::Platform::OpenGL
{
    class OpenGLVertexArray final : public Graphics::Renderer::VertexArray
    {
    public:
        OpenGLVertexArray();

        ~OpenGLVertexArray() override;

        void Bind() const override;
        void UnBind() const override;

        void AddVertexBuffer(std::shared_ptr<Graphics::Renderer::VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(std::shared_ptr<Graphics::Renderer::IndexBuffer>& indexBuffer) override;

        [[nodiscard]] const std::vector<std::shared_ptr<Graphics::Renderer::VertexBuffer>>&
        GetVertexBuffers() const override
        {
            return m_VertexBuffers;
        }

        [[nodiscard]] const std::shared_ptr<Graphics::Renderer::IndexBuffer>& GetIndexBuffer() const override
        {
            return m_IndexBuffer;
        }


        static VertexArray* Create();

    private:
        uint32_t m_RendererId{};
        std::vector<std::shared_ptr<Graphics::Renderer::VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<Graphics::Renderer::IndexBuffer> m_IndexBuffer;
    };
}

