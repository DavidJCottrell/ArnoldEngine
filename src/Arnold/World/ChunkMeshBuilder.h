#pragma once

#include "Arnold/World/Chunk.h"
#include "Arnold/Graphics/Renderer/Mesh.h"

#include <memory>

namespace AE::World
{
    /** Optional neighbor chunks passed to ChunkMeshBuilder::Build for cross-chunk face culling. */
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
         * @brief Build a Mesh from the visible faces of a chunk.
         *
         * A face is only emitted when the neighbouring block is Air or outside the world.
         * Pass neighbor chunks to correctly cull faces at chunk boundaries; a null neighbor
         * pointer means that boundary is a world edge (face is always emitted).
         *
         * @return A Mesh ready for Renderer::Submit, or nullptr if the chunk is entirely Air.
         */
        /** Number of tiles in the texture atlas — must match TextureAtlas::TILE_COUNT. */
        static constexpr int ATLAS_TILE_COUNT = 3;

        static std::shared_ptr<AE::Graphics::Renderer::Mesh> Build(
            const Chunk&          chunk,
            const ChunkNeighbors& neighbors = ChunkNeighbors{});
    };
}
