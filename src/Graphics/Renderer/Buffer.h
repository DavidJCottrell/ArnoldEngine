#pragma once

#include <cstdint>

namespace AE::Graphics::Renderer
{
    /**
     * @class VertexBuffer
     * @brief Interface for managing GPU vertex buffer objects
     *
     * Provides an abstract interface for creating and managing vertex buffers.
     * Handles the storage and transfer of vertex data to the GPU.
     *
     * Features:
     * - Creation of vertex buffers from raw data
     * - Binding and unbinding of buffers
     * - Cross-platform compatibility through renderer abstraction
     */
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer()
        {
        };

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        static VertexBuffer* Create(const float* vertices, uint32_t size);
    };

    /**
     * @class IndexBuffer
     * @brief Interface for managing GPU index buffer objects
     *
     * Provides an abstract interface for creating and managing index buffers.
     * Handles the storage and transfer of index data to the GPU.
     *
     * Features:
     * - Creation of index buffers from raw data
     * - Binding and unbinding of buffers
     * - Access to the number of indices in the buffer
     * - Cross-platform compatibility through renderer abstraction
     */
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer()
        {
        };

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        [[nodiscard]] virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, uint32_t size);
    };
}



