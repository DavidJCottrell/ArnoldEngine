#include "aepch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/RenderingAPI/OpenGL/OpenGLTexture2D.h"

namespace AE::Graphics::Renderer
{
    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<Platform::OpenGL::OpenGLTexture2D>(path);
        default:
            AE_CORE_ERROR("Unknown RendererAPI");
            return nullptr;
        }
    }

    std::shared_ptr<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height, const void* data)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<Platform::OpenGL::OpenGLTexture2D>(width, height, data);
        default:
            AE_CORE_ERROR("Unknown RendererAPI");
            return nullptr;
        }
    }
}
