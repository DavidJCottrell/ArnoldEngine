#include "aepch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace AE::Graphics::Renderer
{
    VertexBuffer* VertexBuffer::Create(const float* vertices, uint32_t size)
    {
        // Decide which rendering API to use
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            AE_CORE_ERROR("RendererAPI::None: not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new Platform::Render::OpenGL::OpenGLVertexBuffer(vertices, size);
        default:
            AE_CORE_ERROR("Error determining rendering API");
            return nullptr;
        }
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, const uint32_t size)
    {
        // Decide which rendering API to use
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            AE_CORE_ERROR("RendererAPI::None: not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new Platform::Render::OpenGL::OpenGLIndexBuffer(indices, size);
        default:
            AE_CORE_ERROR("Error determining rendering API");
            return nullptr;
        }
    }
}
