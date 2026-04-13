#pragma once

#include "Arnold/World/Chunk.h"
#include "Arnold/Graphics/Renderer/Mesh.h"

#include <memory>

namespace AE::World
{
    class ChunkMeshBuilder
    {
    public:
        /**
         * @brief Build a Mesh from the visible faces of a chunk.
         *
         * A face is only emitted when the neighbouring block is Air or out-of-bounds,
         * so internal faces between two solid blocks are culled automatically.
         *
         * @return A Mesh ready for Renderer::Submit, or nullptr if the chunk is entirely Air.
         */
        static std::shared_ptr<AE::Graphics::Renderer::Mesh> Build(const Chunk& chunk);
    };
}
