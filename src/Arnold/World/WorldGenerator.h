#pragma once

#include "Arnold/World/World.h"

#include <cstdint>

namespace AE::World
{
    class WorldGenerator
    {
    public:
        /** Fills the world with a flat single-layer of Grass. */
        static void GenerateFlat(World& world);

        /** Fills the world with a noise-based heightmap: Grass surface, Dirt subsurface, Stone below. */
        static void GenerateTerrain(World& world, uint32_t seed = 0);
    };
}
