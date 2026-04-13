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
        static constexpr int WORLD_SIZE = 16;  // WORLD_SIZE × WORLD_SIZE chunk grid

        World();

        /** Submit all chunk meshes to the renderer. Must be called between BeginScene/EndScene. */
        void Render(const std::shared_ptr<AE::Graphics::Renderer::Material>& material);

        // ----------------------------------------------------------------
        //  Density field access
        // ----------------------------------------------------------------

        /** Trilinear-interpolate density at a floating-point block-index position.
         *  Returns +10 for out-of-bounds below, −10 for out-of-bounds above. */
        float SampleDensity(float wx, float wy, float wz) const;

        /** Add `delta` to all stored density corners within `radius` blocks of `center`
         *  (block-index space). Marks affected chunks dirty for remesh. */
        void ModifyDensity(glm::vec3 center, float radius, float delta);

        // ----------------------------------------------------------------
        //  Chunk access (needed by WorldGenerator)
        // ----------------------------------------------------------------

        Chunk&       GetChunk(int cx, int cz);
        const Chunk& GetChunk(int cx, int cz) const;

        // ----------------------------------------------------------------
        //  Render scale
        // ----------------------------------------------------------------

        void  SetBlockScale(float scale) { m_BlockScale = scale; }
        float GetBlockScale()      const { return m_BlockScale; }

        /** Mark every chunk dirty so meshes are rebuilt on the next Render(). */
        void MarkAllDirty();

    private:
        float m_BlockScale = 1.0f;

        struct ChunkEntry
        {
            Chunk                                          chunk;
            std::shared_ptr<AE::Graphics::Renderer::Mesh> mesh;
            glm::vec3                                      worldPos;
            bool                                           dirty = false;
        };

        ChunkEntry*       GetChunkEntry(int cx, int cz);
        const ChunkEntry* GetChunkEntry(int cx, int cz) const;
        ChunkNeighbors    MakeNeighbors(int cx, int cz);

        // Internal helpers for ModifyDensity
        float GetDensityAtPos(int wx, int wy, int wz) const;
        void  SetDensityAtPos(int wx, int wy, int wz, float v);

        std::vector<ChunkEntry> m_Chunks;
    };
}
