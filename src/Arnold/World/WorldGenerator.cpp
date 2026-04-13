#include "aepch.h"
#include "WorldGenerator.h"
#include "World.h"
#include "WorldNoise.h"

namespace AE::World
{
    void WorldGenerator::GenerateTerrain(World& world, uint32_t seed)
    {
        for (int cx = 0; cx < World::WORLD_SIZE; ++cx)
        for (int cz = 0; cz < World::WORLD_SIZE; ++cz)
        {
            Chunk& chunk = world.GetChunk(cx, cz);
            const int ox = cx * Chunk::SIZE;
            const int oz = cz * Chunk::SIZE;

            for (int lx = 0; lx < Chunk::DENSITY_SIZE; ++lx)
            for (int ly = 0; ly < Chunk::DENSITY_SIZE; ++ly)
            for (int lz = 0; lz < Chunk::DENSITY_SIZE; ++lz)
            {
                const float d = Noise::Density(
                    static_cast<float>(ox + lx),
                    static_cast<float>(ly),
                    static_cast<float>(oz + lz),
                    seed);
                chunk.SetDensity(lx, ly, lz, d);
            }
        }
    }
}
