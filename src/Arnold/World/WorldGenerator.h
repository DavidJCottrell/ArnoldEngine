#pragma once

#include "Arnold/World/World.h"

#include <cstdint>

namespace AE::World
{
    class WorldGenerator
    {
    public:
        /** Fills every chunk's density field with procedural terrain using FBM noise. */
        static void GenerateTerrain(World& world, uint32_t seed = 0);
    };
}
