#include "aepch.h"
#include "ChunkMeshBuilder.h"

#include "Arnold/Graphics/Renderer/Buffer.h"

namespace AE::World
{
    namespace {

        struct FaceDir { int dx, dy, dz; };

        constexpr FaceDir k_Dirs[6] = {
            { 1,  0,  0},  // +X (right)
            {-1,  0,  0},  // -X (left)
            { 0,  1,  0},  // +Y (top)
            { 0, -1,  0},  // -Y (bottom)
            { 0,  0,  1},  // +Z (front)
            { 0,  0, -1},  // -Z (back)
        };

        // 4 vertex offsets per face from the block's (x,y,z) minimum corner.
        // Winding matches Mesh::CreateCube() — CCW when viewed from outside the face.
        constexpr float k_FaceVerts[6][12] = {
            // +X (right face)
            {1,0,1,  1,0,0,  1,1,0,  1,1,1},
            // -X (left face)
            {0,0,0,  0,0,1,  0,1,1,  0,1,0},
            // +Y (top face)
            {0,1,1,  1,1,1,  1,1,0,  0,1,0},
            // -Y (bottom face)
            {0,0,0,  1,0,0,  1,0,1,  0,0,1},
            // +Z (front face)
            {0,0,1,  1,0,1,  1,1,1,  0,1,1},
            // -Z (back face)
            {1,0,0,  0,0,0,  0,1,0,  1,1,0},
        };

        constexpr float k_UVs[4][2] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
        };

        // Outward-facing normal for each of the 6 face directions
        constexpr float k_FaceNormals[6][3] = {
            { 1,  0,  0},  // +X
            {-1,  0,  0},  // -X
            { 0,  1,  0},  // +Y
            { 0, -1,  0},  // -Y
            { 0,  0,  1},  // +Z
            { 0,  0, -1},  // -Z
        };

    } // anonymous namespace

    std::shared_ptr<AE::Graphics::Renderer::Mesh> ChunkMeshBuilder::Build(const Chunk& chunk)
    {
        std::vector<float>    vertices;
        std::vector<uint32_t> indices;
        uint32_t              vertexCount = 0;

        for (int z = 0; z < Chunk::SIZE; ++z)
        for (int y = 0; y < Chunk::SIZE; ++y)
        for (int x = 0; x < Chunk::SIZE; ++x)
        {
            if (chunk.GetBlock(x, y, z) == BlockType::Air)
                continue;

            for (int f = 0; f < 6; ++f)
            {
                const int nx = x + k_Dirs[f].dx;
                const int ny = y + k_Dirs[f].dy;
                const int nz = z + k_Dirs[f].dz;

                // Emit a face only when the neighbour is Air or outside the chunk
                if (chunk.IsInBounds(nx, ny, nz) && chunk.GetBlock(nx, ny, nz) != BlockType::Air)
                    continue;

                for (int v = 0; v < 4; ++v)
                {
                    // Position
                    vertices.push_back(static_cast<float>(x) + k_FaceVerts[f][v * 3 + 0]);
                    vertices.push_back(static_cast<float>(y) + k_FaceVerts[f][v * 3 + 1]);
                    vertices.push_back(static_cast<float>(z) + k_FaceVerts[f][v * 3 + 2]);
                    // UV
                    vertices.push_back(k_UVs[v][0]);
                    vertices.push_back(k_UVs[v][1]);
                    // Normal (same for all 4 vertices on this face)
                    vertices.push_back(k_FaceNormals[f][0]);
                    vertices.push_back(k_FaceNormals[f][1]);
                    vertices.push_back(k_FaceNormals[f][2]);
                }

                indices.insert(indices.end(), {
                    vertexCount,     vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount
                });
                vertexCount += 4;
            }
        }

        if (vertices.empty())
            return nullptr;

        const AE::Graphics::Renderer::BufferLayout layout = {
            {AE::Graphics::Renderer::ShaderDataType::Float3, "a_Position"},
            {AE::Graphics::Renderer::ShaderDataType::Float2, "a_TexCoord"},
            {AE::Graphics::Renderer::ShaderDataType::Float3, "a_Normal"},
        };

        return std::make_shared<AE::Graphics::Renderer::Mesh>(vertices, layout, indices);
    }
}
