#pragma once

#include "Buffer.h"
#include "VertexArray.h"

#include <memory>
#include <vector>

namespace AE::Graphics::Renderer
{
    /**
     * @class Mesh
     * @brief Bundles vertex geometry and index data into a single renderable unit.
     *
     * Mesh owns a VertexArray and exposes static factory methods for common primitives.
     * It intentionally does NOT own a shader or texture — those are rendering concerns
     * kept separate. This is the type that chunk mesh generation will output.
     *
     * Usage:
     * @code
     * // Built-in primitive
     * auto mesh = Mesh::CreateCube();
     *
     * // Dynamic geometry (e.g. chunk mesh)
     * auto mesh = std::make_shared<Mesh>(vertices, layout, indices);
     *
     * Renderer::Submit(shader, mesh, transform);
     * @endcode
     */
    class Mesh
    {
    public:
        /**
         * @brief Construct a mesh from raw geometry data.
         * @param vertices Interleaved vertex data (positions, UVs, normals, etc.)
         * @param layout   Describes the vertex attribute layout
         * @param indices  Triangle index list (3 indices per triangle)
         */
        Mesh(const std::vector<float>& vertices,
             const BufferLayout& layout,
             const std::vector<uint32_t>& indices);

        [[nodiscard]] const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

        // --- Primitive factories ---

        /** Unit cube centred at origin. Layout: Float3 a_Position, Float2 a_TexCoord. */
        static std::shared_ptr<Mesh> CreateCube();

        /** Unit quad in the XY plane, facing +Z. Layout: Float3 a_Position, Float2 a_TexCoord. */
        static std::shared_ptr<Mesh> CreateQuad();

    private:
        std::shared_ptr<VertexArray> m_VertexArray;
    };
}
