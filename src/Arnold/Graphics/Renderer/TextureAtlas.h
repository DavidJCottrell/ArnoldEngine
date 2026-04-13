#pragma once

#include "Arnold/Graphics/Renderer/Texture.h"

#include <memory>
#include <string>

namespace AE::Graphics::Renderer
{
    /**
     * Horizontal-strip texture atlas: [Grass | Dirt | Stone]
     * Tile index: Grass=0, Dirt=1, Stone=2
     *
     * Grass and Dirt are loaded from disk; Stone is generated procedurally.
     * The resulting Texture2D is set on a Material exactly like any other texture.
     */
    class TextureAtlas
    {
    public:
        static constexpr int TILE_COUNT = 3;  // Grass=0, Dirt=1, Stone=2

        /**
         * Builds the atlas from grass and dirt image paths.
         * Tile size = min(grass, dirt) source dimension, capped at 256.
         */
        static std::shared_ptr<TextureAtlas> Create(
            const std::string& grassPath,
            const std::string& dirtPath);

        const std::shared_ptr<Texture2D>& GetTexture() const { return m_Texture; }

    private:
        std::shared_ptr<Texture2D> m_Texture;
    };
}
