#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <cstdint>

namespace AE::World::Noise
{
    // ----------------------------------------------------------------
    //  Low-level hash & interpolation
    // ----------------------------------------------------------------

    inline float Hash2D(int x, int z, uint32_t seed)
    {
        uint32_t h = static_cast<uint32_t>(x) * 1619u
                   + static_cast<uint32_t>(z) * 31337u
                   + seed * 1000003u;
        h ^= h >> 16; h *= 0x45d9f3bu; h ^= h >> 16;
        return static_cast<float>(h & 0xFFFFu) / 65535.0f;
    }

    inline float Hash3D(int x, int y, int z)
    {
        uint32_t h = static_cast<uint32_t>(x) * 1619u
                   + static_cast<uint32_t>(y) * 31337u
                   + static_cast<uint32_t>(z) * 6271u;
        h ^= h >> 16; h *= 0x45d9f3bu; h ^= h >> 16;
        return static_cast<float>(h & 0xFFFFu) / 65535.0f;
    }

    inline float Smoothstep(float t) { return t * t * (3.0f - 2.0f * t); }

    inline float ValueNoise2D(float wx, float wz, uint32_t seed)
    {
        int   ix = static_cast<int>(std::floor(wx));
        int   iz = static_cast<int>(std::floor(wz));
        float fx = wx - ix, fz = wz - iz;
        float ux = Smoothstep(fx), uz = Smoothstep(fz);
        return glm::mix(glm::mix(Hash2D(ix,   iz,   seed), Hash2D(ix+1, iz,   seed), ux),
                        glm::mix(Hash2D(ix,   iz+1, seed), Hash2D(ix+1, iz+1, seed), ux), uz);
    }

    inline float Fbm2D(float wx, float wz, uint32_t seed)
    {
        float v = 0.f, amp = 0.5f, freq = 1.f;
        for (int i = 0; i < 4; ++i) {
            v    += amp * ValueNoise2D(wx * freq, wz * freq, seed + static_cast<uint32_t>(i) * 997u);
            amp  *= 0.5f; freq *= 2.f;
        }
        return v;
    }

    inline float TrilinearNoise(float wx, float wy, float wz)
    {
        int   ix = (int)std::floor(wx), iy = (int)std::floor(wy), iz = (int)std::floor(wz);
        float fx = wx-ix, fy = wy-iy, fz = wz-iz;
        float ux = Smoothstep(fx), uy = Smoothstep(fy), uz = Smoothstep(fz);
        auto h = Hash3D;
        float x00 = glm::mix(h(ix,iy,  iz), h(ix+1,iy,  iz), ux);
        float x10 = glm::mix(h(ix,iy+1,iz), h(ix+1,iy+1,iz), ux);
        float x01 = glm::mix(h(ix,iy,  iz+1), h(ix+1,iy,  iz+1), ux);
        float x11 = glm::mix(h(ix,iy+1,iz+1), h(ix+1,iy+1,iz+1), ux);
        return glm::mix(glm::mix(x00,x10,uy), glm::mix(x01,x11,uy), uz);
    }

    // ----------------------------------------------------------------
    //  Terrain density field
    //    > 0  →  solid      < 0  →  air      isosurface at 0
    // ----------------------------------------------------------------
    inline float Density(float wx, float wy, float wz, uint32_t seed)
    {
        // 2-D FBM base (same params as WorldGenerator terrain)
        const float n2D     = Fbm2D(wx * 0.05f, wz * 0.05f, seed);
        const float surface = 2.0f + n2D * 7.0f;   // minH=2, maxH=9

        // 3-D warp for organic detail
        float warp = 0.f, amp = 0.5f, freq = 1.f;
        for (int i = 0; i < 3; ++i) {
            warp += amp * (TrilinearNoise(wx*freq*0.25f, wy*freq*0.25f, wz*freq*0.25f)*2.f - 1.f);
            amp *= 0.5f; freq *= 2.f;
        }
        warp *= 0.8f;

        return (surface + warp) - wy;
    }

    // ----------------------------------------------------------------
    //  Vertex colour (height + slope blending)
    // ----------------------------------------------------------------
    inline glm::vec4 VertexColor(float localY, const glm::vec3& normal)
    {
        const float ny          = std::abs(normal.y);
        const float heightBlend = glm::clamp(localY / 9.0f, 0.0f, 1.0f);
        const float slopeBlend  = glm::clamp((ny - 0.5f) / 0.5f, 0.0f, 1.0f);
        const float grassBlend  = heightBlend * slopeBlend;

        const glm::vec3 grass = {0.28f, 0.52f, 0.18f};
        const glm::vec3 dirt  = {0.40f, 0.28f, 0.16f};
        const glm::vec3 stone = {0.50f, 0.48f, 0.46f};

        return glm::vec4(glm::mix(stone, glm::mix(dirt, grass, grassBlend), slopeBlend), 1.0f);
    }

} // namespace AE::World::Noise
