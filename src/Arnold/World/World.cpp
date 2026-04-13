#include "aepch.h"
#include "World.h"

#include "Arnold/World/ChunkMeshBuilder.h"
#include "Arnold/World/Frustum.h"
#include "Arnold/Graphics/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

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

    // ----------------------------------------------------------------
    //  Rendering
    // ----------------------------------------------------------------

    void World::Render(const std::shared_ptr<AE::Graphics::Renderer::Material>& material,
                       const glm::mat4& viewProjection)
    {
        // ----------------------------------------------------------------
        //  Rebuild dirty chunk meshes (synchronous)
        // ----------------------------------------------------------------
        for (int cx = 0; cx < WORLD_SIZE; ++cx)
        for (int cz = 0; cz < WORLD_SIZE; ++cz)
        {
            ChunkEntry* e = GetChunkEntry(cx, cz);
            if (!e->dirty) continue;
            e->mesh  = ChunkMeshBuilder::Build(e->chunk, MakeNeighbors(cx, cz));
            e->dirty = false;
        }

        // ----------------------------------------------------------------
        //  Frustum-cull and submit visible meshes
        // ----------------------------------------------------------------
        const auto planes = Frustum::ExtractPlanes(viewProjection);

        for (int cx = 0; cx < WORLD_SIZE; ++cx)
        for (int cz = 0; cz < WORLD_SIZE; ++cz)
        {
            ChunkEntry* e = GetChunkEntry(cx, cz);
            if (!e->mesh) continue;

            const glm::vec3 wMin = e->worldPos * m_BlockScale;
            const glm::vec3 wMax = (e->worldPos + glm::vec3(Chunk::SIZE)) * m_BlockScale;
            if (!Frustum::IsAABBVisible(planes, wMin, wMax)) continue;

            const glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), wMin)
              * glm::scale(glm::mat4(1.0f), glm::vec3(m_BlockScale));
            AE::Graphics::Renderer::Renderer::Submit(material, e->mesh, transform);
        }
    }

    // ----------------------------------------------------------------
    //  Chunk access
    // ----------------------------------------------------------------

    Chunk& World::GetChunk(int cx, int cz)
    {
        return m_Chunks[cx * WORLD_SIZE + cz].chunk;
    }
    const Chunk& World::GetChunk(int cx, int cz) const
    {
        return m_Chunks[cx * WORLD_SIZE + cz].chunk;
    }

    World::ChunkEntry* World::GetChunkEntry(int cx, int cz)
    {
        if (cx < 0 || cx >= WORLD_SIZE || cz < 0 || cz >= WORLD_SIZE) return nullptr;
        return &m_Chunks[cx * WORLD_SIZE + cz];
    }
    const World::ChunkEntry* World::GetChunkEntry(int cx, int cz) const
    {
        if (cx < 0 || cx >= WORLD_SIZE || cz < 0 || cz >= WORLD_SIZE) return nullptr;
        return &m_Chunks[cx * WORLD_SIZE + cz];
    }

    void World::MarkAllDirty()
    {
        for (auto& entry : m_Chunks)
            entry.dirty = true;
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

    // ----------------------------------------------------------------
    //  Density helpers
    // ----------------------------------------------------------------

    float World::GetDensityAtPos(int wx, int wy, int wz) const
    {
        if (wy < 0) return  10.0f;   // below world = solid
        if (wy > Chunk::SIZE) return -10.0f;  // above world = air
        const int cx = wx / Chunk::SIZE;
        const int cz = wz / Chunk::SIZE;
        const int lx = wx % Chunk::SIZE;
        const int lz = wz % Chunk::SIZE;
        const ChunkEntry* e = GetChunkEntry(cx, cz);
        if (!e) return -10.0f;
        return e->chunk.GetDensity(lx, wy, lz);
    }

    void World::SetDensityAtPos(int wx, int wy, int wz, float v)
    {
        if (wy < 0 || wy > Chunk::SIZE) return;

        auto update = [&](int cx, int cz, int lx, int lz) {
            if (cx < 0 || cx >= WORLD_SIZE || cz < 0 || cz >= WORLD_SIZE) return;
            if (lx < 0 || lx > Chunk::SIZE || lz < 0 || lz > Chunk::SIZE) return;
            if (auto* e = GetChunkEntry(cx, cz)) {
                e->chunk.SetDensity(lx, wy, lz, v);
                e->dirty = true;
            }
        };

        const int cx = wx / Chunk::SIZE;
        const int cz = wz / Chunk::SIZE;
        const int lx = wx % Chunk::SIZE;
        const int lz = wz % Chunk::SIZE;

        // Update the owning chunk
        update(cx, cz, lx, lz);

        // Also update the boundary copies stored in neighboring chunks
        if (lx == 0 && cx > 0) update(cx - 1, cz, Chunk::SIZE, lz);
        if (lz == 0 && cz > 0) update(cx, cz - 1, lx, Chunk::SIZE);
        if (lx == 0 && lz == 0 && cx > 0 && cz > 0)
            update(cx - 1, cz - 1, Chunk::SIZE, Chunk::SIZE);

        // Mark +X / +Z neighbors dirty when the edited voxel falls within the
        // gradient sample window (h=0.5) of those chunks' boundary vertices.
        if (lx == Chunk::SIZE - 1)
            if (auto* e = GetChunkEntry(cx + 1, cz)) e->dirty = true;
        if (lz == Chunk::SIZE - 1)
            if (auto* e = GetChunkEntry(cx, cz + 1)) e->dirty = true;
        if (lx == Chunk::SIZE - 1 && lz == Chunk::SIZE - 1)
            if (auto* e = GetChunkEntry(cx + 1, cz + 1)) e->dirty = true;
    }

    float World::SampleDensity(float wx, float wy, float wz) const
    {
        if (wy < 0.0f) return  10.0f;
        if (wy > static_cast<float>(Chunk::SIZE)) return -10.0f;

        const int ix = static_cast<int>(std::floor(wx));
        const int iy = static_cast<int>(std::floor(wy));
        const int iz = static_cast<int>(std::floor(wz));
        const float fx = wx - ix, fy = wy - iy, fz = wz - iz;

        auto G = [&](int x, int y, int z) { return GetDensityAtPos(x, y, z); };

        return G(ix,  iy,  iz  )*(1-fx)*(1-fy)*(1-fz)
             + G(ix+1,iy,  iz  )*   fx *(1-fy)*(1-fz)
             + G(ix,  iy+1,iz  )*(1-fx)*   fy *(1-fz)
             + G(ix+1,iy+1,iz  )*   fx *   fy *(1-fz)
             + G(ix,  iy,  iz+1)*(1-fx)*(1-fy)*   fz
             + G(ix+1,iy,  iz+1)*   fx *(1-fy)*   fz
             + G(ix,  iy+1,iz+1)*(1-fx)*   fy *   fz
             + G(ix+1,iy+1,iz+1)*   fx *   fy *   fz;
    }

    void World::ModifyDensity(glm::vec3 center, float radius, float delta)
    {
        const int iR = static_cast<int>(std::ceil(radius));
        const glm::ivec3 c = glm::ivec3(glm::round(center));

        for (int dx = -iR; dx <= iR; ++dx)
        for (int dy = -iR; dy <= iR; ++dy)
        for (int dz = -iR; dz <= iR; ++dz)
        {
            if (glm::length(glm::vec3(dx, dy, dz)) > radius) continue;
            const int wx = c.x + dx, wy = c.y + dy, wz = c.z + dz;
            const float cur = GetDensityAtPos(wx, wy, wz);
            SetDensityAtPos(wx, wy, wz, cur + delta);
        }
    }
}
