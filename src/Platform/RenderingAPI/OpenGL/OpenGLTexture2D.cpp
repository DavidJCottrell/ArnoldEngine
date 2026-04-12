#include "aepch.h"
#include "OpenGLTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "stb_image.h"

namespace AE::Platform::OpenGL
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!pixels)
        {
            AE_CORE_ERROR("Failed to load texture: {0}", path);
            return;
        }

        const GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        Upload(static_cast<uint32_t>(width), static_cast<uint32_t>(height), format, pixels);

        stbi_image_free(pixels);
    }

    OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height, const void* data)
    {
        Upload(width, height, GL_RGBA, data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(const uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void OpenGLTexture2D::Upload(const uint32_t width, const uint32_t height,
                                  const GLenum format, const void* data)
    {
        m_Width  = width;
        m_Height = height;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format),
                     static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                     0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}
