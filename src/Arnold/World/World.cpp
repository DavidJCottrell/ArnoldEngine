#include "aepch.h"
#include "World.h"

#include "Arnold/World/ChunkMeshBuilder.h"
#include "Arnold/Graphics/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace AE::World
{
    World::World()
    {
        m_Chunks.reserve(WORLD_SIZE * WORLD_SIZE);
        for (int cx = 0; cx < WORLD_SIZE; ++cx)
        for (int cz = 0; cz < WORLD_SIZE; ++cz)
        {
            ChunkEntry entry;
            entry.worldPos = glm::vec3(cx * Chunk::SIZE, 0.0f, cz * Chunk::SIZE);
            m_Chunks.push_back(std::move(entry));
        }
    }

    void World::Render(const std::shared_ptr<AE::Graphics::Renderer::Material>& material)
    {
        for (int cx = 0; cx < WORLD_SIZE; ++cx)
        for (int cz = 0; cz < WORLD_SIZE; ++cz)
        {
            ChunkEntry* entry = GetChunkEntry(cx, cz);

            if (entry->dirty)
            {
                entry->mesh  = ChunkMeshBuilder::Build(entry->chunk, MakeNeighbors(cx, cz));
                entry->dirty = false;
            }

            if (!entry->mesh)
                continue;

            const glm::mat4 transform = glm::translate(glm::mat4(1.0f), entry->worldPos);
            AE::Graphics::Renderer::Renderer::Submit(material, entry->mesh, transform);
        }
    }

    BlockType World::GetBlock(int worldX, int worldY, int worldZ) const
    {
        if (worldX < 0 || worldY < 0 || worldZ < 0 || worldY >= Chunk::SIZE)
            return BlockType::Air;
        const int cx = worldX / Chunk::SIZE;
        const int cz = worldZ / Chunk::SIZE;
        const ChunkEntry* entry = const_cast<World*>(this)->GetChunkEntry(cx, cz);
        if (!entry) return BlockType::Air;
        return entry->chunk.GetBlock(worldX % Chunk::SIZE, worldY, worldZ % Chunk::SIZE);
    }

    void World::SetBlock(int worldX, int worldY, int worldZ, BlockType type)
    {
        if (worldX < 0 || worldY < 0 || worldZ < 0 || worldY >= Chunk::SIZE)
            return;
        const int cx = worldX / Chunk::SIZE;
        const int cz = worldZ / Chunk::SIZE;
        ChunkEntry* entry = GetChunkEntry(cx, cz);
        if (!entry) return;
        const int lx = worldX % Chunk::SIZE;
        const int lz = worldZ % Chunk::SIZE;
        if (!entry->chunk.IsInBounds(lx, worldY, lz)) return;
        entry->chunk.SetBlock(lx, worldY, lz, type);
        entry->dirty = true;

        // If the changed block is on a chunk boundary, the neighbor must also remesh
        if (lx == 0)               if (auto* nbr = GetChunkEntry(cx - 1, cz)) nbr->dirty = true;
        if (lx == Chunk::SIZE - 1) if (auto* nbr = GetChunkEntry(cx + 1, cz)) nbr->dirty = true;
        if (lz == 0)               if (auto* nbr = GetChunkEntry(cx, cz - 1)) nbr->dirty = true;
        if (lz == Chunk::SIZE - 1) if (auto* nbr = GetChunkEntry(cx, cz + 1)) nbr->dirty = true;
    }

    World::ChunkEntry* World::GetChunkEntry(int cx, int cz)
    {
        if (cx < 0 || cx >= WORLD_SIZE || cz < 0 || cz >= WORLD_SIZE)
            return nullptr;
        return &m_Chunks[cx * WORLD_SIZE + cz];
    }

    ChunkNeighbors World::MakeNeighbors(int cx, int cz)
    {
        ChunkNeighbors n;
        if (auto* e = GetChunkEntry(cx + 1, cz)) n.px = &e->chunk;
        if (auto* e = GetChunkEntry(cx - 1, cz)) n.nx = &e->chunk;
        if (auto* e = GetChunkEntry(cx, cz + 1)) n.pz = &e->chunk;
        if (auto* e = GetChunkEntry(cx, cz - 1)) n.nz = &e->chunk;
        return n;
    }
}
