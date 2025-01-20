#pragma once
#include "Arnold/Graphics/Renderer/Buffer.h"

#include "Arnold/Graphics/Renderer/VertexArray.h"

namespace AE::Platform::OpenGL
{
    /**
     * @class OpenGLVertexArray
     * @brief OpenGL-specific implementation of vertex array objects
     *
     * Implements vertex array management using OpenGL:
     * - Creates and manages OpenGL Vertex Array Objects (VAO)
     * - Configures vertex attributes based on buffer layouts
     * - Handles binding of vertex and index buffers
     * - Manages OpenGL state for rendering
     *
     * Each OpenGLVertexArray maintains:
     * - A list of attached vertex buffers with their layouts
     * - An optional index buffer for indexed rendering
     * - OpenGL state management for the VAO
     *
     * Example usage:
     * @code
     * auto vertexArray = std::make_shared<OpenGLVertexArray>();
     * vertexArray->AddVertexBuffer(vertexBuffer);
     * vertexArray->SetIndexBuffer(indexBuffer);
     * vertexArray->Bind();
     * // Draw commands...
     * vertexArray->Unbind();
     * @endcode
     */
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

