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

            for (int x = 0; x < Chunk::SIZE; ++x)
            for (int z = 0; z < Chunk::SIZE; ++z)
                entry.chunk.SetBlock(x, 0, z, BlockType::Grass);

            entry.mesh     = ChunkMeshBuilder::Build(entry.chunk);
            entry.worldPos = glm::vec3(cx * Chunk::SIZE, 0.0f, cz * Chunk::SIZE);

            m_Chunks.push_back(std::move(entry));
        }
    }

    void World::Render(const std::shared_ptr<AE::Graphics::Renderer::Shader>& shader)
    {
        for (const auto& entry : m_Chunks)
        {
            if (!entry.mesh)
                continue;

            const glm::mat4 transform = glm::translate(glm::mat4(1.0f), entry.worldPos);
            AE::Graphics::Renderer::Renderer::Submit(shader, entry.mesh, transform);
        }
    }
}
