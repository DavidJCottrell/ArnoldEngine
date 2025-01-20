#include "aepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace AE::Graphics::Renderer
{
    VertexArray::~VertexArray() = default;

    VertexArray* VertexArray::Create()
    {
        // Decide which rendering API to use
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            AE_CORE_ERROR("RendererAPI::None: not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new Platform::OpenGL::OpenGLVertexArray();
        default:
            AE_CORE_ERROR("Error determining rendering API");
            return nullptr;
        }
    }
}
