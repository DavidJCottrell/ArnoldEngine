#pragma once

#include "Arnold/World/Chunk.h"
#include "Arnold/World/ChunkMeshBuilder.h"
#include "Arnold/Graphics/Renderer/Material.h"
#include "Arnold/Graphics/Renderer/Mesh.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace AE::World
{
    class World
    {
    public:
        static constexpr int WORLD_SIZE = 4;  // WORLD_SIZE x WORLD_SIZE chunk grid

        /** Generates a flat grass world. Chunk meshes are built immediately. */
        World();

        /** Submit all chunk meshes to the renderer. Must be called between BeginScene/EndScene. */
        void Render(const std::shared_ptr<AE::Graphics::Renderer::Material>& material);

        /** Returns the block at world coordinates, or BlockType::Air if out of bounds. */
        BlockType GetBlock(int worldX, int worldY, int worldZ) const;

        /** Sets the block at world coordinates. Marks the owning chunk dirty for rebuild. */
        void SetBlock(int worldX, int worldY, int worldZ, BlockType type);

    private:
        struct ChunkEntry
        {
            Chunk                                              chunk;
            std::shared_ptr<AE::Graphics::Renderer::Mesh>     mesh;
            glm::vec3                                          worldPos;
            bool                                               dirty = false;
        };

        /** Returns a pointer to the ChunkEntry at grid position (cx, cz), or nullptr if out of bounds. */
        ChunkEntry* GetChunkEntry(int cx, int cz);

        /** Builds a Neighbors struct for chunk (cx, cz) from the live chunk grid. */
        ChunkNeighbors MakeNeighbors(int cx, int cz);

        std::vector<ChunkEntry> m_Chunks;
    };
}
