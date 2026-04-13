#pragma once

#include "Arnold/World/Chunk.h"
#include "Arnold/Graphics/Renderer/Mesh.h"

#include <memory>

namespace AE::World
{
    /** Neighbor chunks supplied to ChunkMeshBuilder::Build for seamless boundary rendering. */
    struct ChunkNeighbors
    {
        const Chunk* px = nullptr;  // +X neighbor
        const Chunk* nx = nullptr;  // -X neighbor
        const Chunk* pz = nullptr;  // +Z neighbor
        const Chunk* nz = nullptr;  // -Z neighbor
    };

    class ChunkMeshBuilder
    {
    public:
        /**
         * @brief Build a smooth mesh from the chunk's stored density field using marching cubes.
         *
         * Iterates all SIZE³ cells. Each cell reads corners from the chunk's 17³ density array,
         * which already includes a one-voxel overlap with the neighboring chunks — so
         * ChunkNeighbors are only needed for the gradient-normal computation at boundary vertices.
         *
         * @return A Mesh ready for Renderer::Submit, or nullptr if the chunk is entirely solid/air.
         */
        static std::shared_ptr<AE::Graphics::Renderer::Mesh> Build(
            const Chunk&          chunk,
            const ChunkNeighbors& neighbors = ChunkNeighbors{});
    };
}
