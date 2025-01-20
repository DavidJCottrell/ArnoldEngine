#pragma once

#include "Buffer.h"

namespace AE::Graphics::Renderer
{
    /**
     * @class VertexArray
     * @brief Abstract interface for managing vertex array objects (VAOs)
     *
     * The VertexArray class provides a generic interface for:
     * - Creating and managing vertex array objects
     * - Binding vertex buffers with specific layouts
     * - Managing index buffers for indexed rendering
     * - Cross-platform vertex array management
     *
     * Key features:
     * - Multiple vertex buffer support
     * - Automatic layout management
     * - Index buffer integration
     * - Platform-independent interface
     *
     * @note Concrete implementations exist for different graphics APIs (OpenGL, etc.)
     */
    class VertexArray
    {
    public:
        virtual ~VertexArray();

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        [[nodiscard]] virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        [[nodiscard]] virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;


        static VertexArray* Create();
    };
}
