#include "aepch.h"
#include "Mesh.h"

namespace AE::Graphics::Renderer
{
    Mesh::Mesh(const std::vector<float>& vertices,
               const BufferLayout& layout,
               const std::vector<uint32_t>& indices)
    {
        m_VertexArray.reset(VertexArray::Create());

        auto vb = std::shared_ptr<VertexBuffer>(
            VertexBuffer::Create(vertices.data(),
                                 static_cast<uint32_t>(vertices.size() * sizeof(float))));
        vb->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vb);

        auto ib = std::shared_ptr<IndexBuffer>(
            IndexBuffer::Create(indices.data(),
                                static_cast<uint32_t>(indices.size())));
        m_VertexArray->SetIndexBuffer(ib);
    }

    std::shared_ptr<Mesh> Mesh::CreateCube()
    {
        // 24 vertices: 4 per face x 6 faces
        // Layout per vertex: Float3 position, Float2 UV
        // UVs: (0,0) bottom-left to (1,1) top-right, consistent across all faces
        const std::vector<float> vertices = {
            // Front (+Z)
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
            // Back (-Z)
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
            // Top (+Y)
            -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
            // Bottom (-Y)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
            // Right (+X)
             0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
            // Left (-X)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        };

        // 36 indices: 6 faces x 2 triangles x 3 vertices
        std::vector<uint32_t> indices;
        indices.reserve(36);
        for (uint32_t face = 0; face < 6; ++face)
        {
            const uint32_t base = face * 4;
            indices.insert(indices.end(), {base, base + 1, base + 2,
                                           base + 2, base + 3, base});
        }

        const BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" },
        };

        return std::make_shared<Mesh>(vertices, layout, indices);
    }

    std::shared_ptr<Mesh> Mesh::CreateQuad()
    {
        // Unit quad in the XY plane, facing +Z
        const std::vector<float> vertices = {
            -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f,   0.0f, 1.0f,
        };

        const std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

        const BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" },
        };

        return std::make_shared<Mesh>(vertices, layout, indices);
    }
}
