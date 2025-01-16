#pragma once

#include "glm/fwd.hpp"
#include "Graphics/Renderer/Buffer.h"

namespace AE::Platform::Render::OpenGL
{
    // -------------------- VertexBuffer --------------------
    // OpenGL implementation of generic VertexBuffer
    class OpenGLVertexBuffer final : public Graphics::Renderer::VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);

        virtual ~OpenGLVertexBuffer();
        void Bind() const override;
        void UnBind() const override;

    private:
        uint32_t m_RendererID;
    };

    // -------------------- IndexBuffer --------------------
    // OpenGL implementation of generic IndexBuffer
    class OpenGLIndexBuffer final : public Graphics::Renderer::IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);

        virtual ~OpenGLIndexBuffer();
        void Bind() const override;
        void UnBind() const override;

        [[nodiscard]] uint32_t GetCount() const override
        {
            return m_Count;
        };

    private:
        uint32_t m_RendererID;
        uint32_t m_Count{};
    };
}
