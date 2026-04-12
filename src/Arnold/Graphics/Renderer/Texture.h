#pragma once

#include <memory>
#include <string>
#include <cstdint>

namespace AE::Graphics::Renderer
{
    /**
     * @class Texture2D
     * @brief Abstract interface for 2D textures
     *
     * Provides a platform-agnostic API for creating and binding 2D textures.
     * Supports both file-based loading and construction from raw pixel data.
     *
     * Usage:
     * @code
     * // From file
     * auto tex = Texture2D::Create("assets/grass.png");
     *
     * // From raw RGBA data
     * auto tex = Texture2D::Create(width, height, pixelData);
     *
     * // Bind to a texture slot before rendering
     * tex->Bind(0);
     * shader->UploadUniformInt("u_Texture", 0);
     * @endcode
     */
    class Texture2D
    {
    public:
        virtual ~Texture2D() = default;

        [[nodiscard]] virtual uint32_t GetWidth()  const = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        /**
         * @brief Binds the texture to the given texture slot
         * @param slot OpenGL texture unit index (0–15)
         */
        virtual void Bind(uint32_t slot = 0) const = 0;

        /**
         * @brief Load a texture from an image file (PNG, JPG, BMP, etc.)
         * @param path Path to the image file, relative to the working directory
         */
        static std::shared_ptr<Texture2D> Create(const std::string& path);

        /**
         * @brief Create a texture from raw RGBA pixel data
         * @param width  Texture width in pixels
         * @param height Texture height in pixels
         * @param data   Pointer to RGBA pixel data (4 bytes per pixel)
         */
        static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height, const void* data);
    };
}
