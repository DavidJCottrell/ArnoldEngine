#include "aepch.h"
#include "TextureAtlas.h"

// stb_image.h — implementation already compiled in OpenGLTexture2D.cpp
#include "stb_image.h"

#include <algorithm>
#include <cstring>
#include <vector>

namespace AE::Graphics::Renderer
{
    std::shared_ptr<TextureAtlas> TextureAtlas::Create(
        const std::string& grassPath,
        const std::string& dirtPath)
    {
        stbi_set_flip_vertically_on_load(true);

        int gw, gh, gc, dw, dh, dc;
        stbi_uc* grassPixels = stbi_load(grassPath.c_str(), &gw, &gh, &gc, 4);
        stbi_uc* dirtPixels  = stbi_load(dirtPath.c_str(),  &dw, &dh, &dc, 4);

        if (!grassPixels) AE_CORE_ERROR("TextureAtlas: failed to load {0}", grassPath);
        if (!dirtPixels)  AE_CORE_ERROR("TextureAtlas: failed to load {0}", dirtPath);

        const uint32_t tileSize = std::min({
            static_cast<uint32_t>(gw), static_cast<uint32_t>(gh),
            static_cast<uint32_t>(dw), static_cast<uint32_t>(dh),
            256u
        });

        const uint32_t atlasW = tileSize * TILE_COUNT;
        const uint32_t atlasH = tileSize;
        std::vector<uint8_t> atlas(atlasW * atlasH * 4, 0);

        // Copy top-left tileSize×tileSize region from source image into atlas tile slot
        auto copyTile = [&](const stbi_uc* src, int srcW, int tileX)
        {
            for (uint32_t row = 0; row < tileSize; ++row)
            {
                const uint8_t* srcRow = src + row * srcW * 4;
                uint8_t*       dstRow = atlas.data() + row * atlasW * 4 + tileX * tileSize * 4;
                std::memcpy(dstRow, srcRow, tileSize * 4);
            }
        };

        if (grassPixels) copyTile(grassPixels, gw, 0);
        if (dirtPixels)  copyTile(dirtPixels,  dw, 1);

        stbi_image_free(grassPixels);
        stbi_image_free(dirtPixels);

        // Tile 2: procedural stone — gray with hash-based variation
        auto stoneHash = [](uint32_t x, uint32_t y) -> uint8_t
        {
            uint32_t h = x * 1619u + y * 31337u;
            h ^= h >> 16;
            h *= 0x45d9f3bu;
            h ^= h >> 16;
            return static_cast<uint8_t>(128 + (h & 31));  // gray in [128, 159]
        };

        for (uint32_t row = 0; row < tileSize; ++row)
        for (uint32_t col = 0; col < tileSize; ++col)
        {
            const uint8_t v  = stoneHash(col, row);
            uint8_t*      px = atlas.data() + (row * atlasW + 2 * tileSize + col) * 4;
            px[0] = v; px[1] = v; px[2] = v; px[3] = 255;
        }

        auto obj      = std::make_shared<TextureAtlas>();
        obj->m_Texture = Texture2D::Create(atlasW, atlasH, atlas.data());
        return obj;
    }
}
