#include "aepch.h"
#include "WorldGenerator.h"

#include <glm/glm.hpp>
#include <cmath>

namespace AE::World
{
    namespace {

        // Integer hash → float in [0, 1)
        float hash2D(int x, int z, uint32_t seed)
        {
            uint32_t h = static_cast<uint32_t>(x) * 1619u
                       + static_cast<uint32_t>(z) * 31337u
                       + seed * 1000003u;
            h ^= h >> 16;
            h *= 0x45d9f3bu;
            h ^= h >> 16;
            return static_cast<float>(h & 0xFFFFu) / 65535.0f;
        }

        float smoothstep(float t) { return t * t * (3.0f - 2.0f * t); }

        // Bilinear value noise
        float valueNoise(float wx, float wz, uint32_t seed)
        {
            const int   ix = static_cast<int>(std::floor(wx));
            const int   iz = static_cast<int>(std::floor(wz));
            const float fx = wx - static_cast<float>(ix);
            const float fz = wz - static_cast<float>(iz);
            const float ux = smoothstep(fx);
            const float uz = smoothstep(fz);

            const float v00 = hash2D(ix,     iz,     seed);
            const float v10 = hash2D(ix + 1, iz,     seed);
            const float v01 = hash2D(ix,     iz + 1, seed);
            const float v11 = hash2D(ix + 1, iz + 1, seed);

            return glm::mix(glm::mix(v00, v10, ux),
                            glm::mix(v01, v11, ux), uz);
        }

        // 4-octave fractal Brownian motion, output ≈ [0, 1)
        float fbm(float wx, float wz, uint32_t seed)
        {
            float value = 0.0f, amplitude = 0.5f, frequency = 1.0f;
            for (int i = 0; i < 4; ++i)
            {
                value     += amplitude * valueNoise(wx * frequency, wz * frequency, seed + static_cast<uint32_t>(i) * 997u);
                amplitude *= 0.5f;
                frequency *= 2.0f;
            }
            return value;
        }

    } // anonymous namespace

    void WorldGenerator::GenerateFlat(World& world)
    {
        const int total = World::WORLD_SIZE * Chunk::SIZE;
        for (int wx = 0; wx < total; ++wx)
        for (int wz = 0; wz < total; ++wz)
            world.SetBlock(wx, 0, wz, BlockType::Grass);
    }

    void WorldGenerator::GenerateTerrain(World& world, uint32_t seed)
    {
        const int   total     = World::WORLD_SIZE * Chunk::SIZE;
        const float scale     = 0.05f;
        const int   minHeight = 2;
        const int   maxHeight = 9;  // well within Chunk::SIZE = 16

        for (int wx = 0; wx < total; ++wx)
        for (int wz = 0; wz < total; ++wz)
        {
            const float noise  = fbm(static_cast<float>(wx) * scale,
                                     static_cast<float>(wz) * scale, seed);
            const int   height = minHeight + static_cast<int>(noise * static_cast<float>(maxHeight - minHeight));

            for (int wy = 0; wy < height; ++wy)
            {
                BlockType type;
                if      (wy == height - 1)   type = BlockType::Grass;
                else if (wy >= height - 3)   type = BlockType::Dirt;
                else                         type = BlockType::Stone;

                world.SetBlock(wx, wy, wz, type);
            }
        }
    }
}
