#pragma once

#include "Arnold/World/Chunk.h"
#include "Arnold/Graphics/Renderer/Mesh.h"
#include "Arnold/Graphics/Renderer/Shader.h"

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
        void Render(const std::shared_ptr<AE::Graphics::Renderer::Shader>& shader);

    private:
        struct ChunkEntry
        {
            Chunk                                              chunk;
            std::shared_ptr<AE::Graphics::Renderer::Mesh>     mesh;
            glm::vec3                                          worldPos;
        };

        std::vector<ChunkEntry> m_Chunks;
    };
}
