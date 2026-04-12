#pragma once

#include "Arnold/Graphics/Renderer/Texture.h"
#include <glad/glad.h>

namespace AE::Platform::OpenGL
{
    class OpenGLTexture2D : public Graphics::Renderer::Texture2D
    {
    public:
        explicit OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(uint32_t width, uint32_t height, const void* data);
        ~OpenGLTexture2D() override;

        [[nodiscard]] uint32_t GetWidth()  const override { return m_Width; }
        [[nodiscard]] uint32_t GetHeight() const override { return m_Height; }

        void Bind(uint32_t slot = 0) const override;

    private:
        void Upload(uint32_t width, uint32_t height, GLenum format, const void* data);

        uint32_t m_RendererID{};
        uint32_t m_Width{};
        uint32_t m_Height{};
    };
}
