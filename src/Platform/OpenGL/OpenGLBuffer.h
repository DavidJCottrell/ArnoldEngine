#pragma once

#include "glm/fwd.hpp"
#include "Arnold/Graphics/Renderer/Buffer.h"

namespace AE::Platform::OpenGL
{
    // -------------------- VertexBuffer --------------------
    /**
     * @class OpenGLVertexBuffer
     * @brief OpenGL-specific implementation of the vertex buffer interface
     *
     * Provides OpenGL implementation for storing and managing vertex data on the GPU:
     * - Creates and manages OpenGL vertex buffer objects (VBOs)
     * - Handles buffer data transfer to GPU memory
     * - Supports different vertex layouts through BufferLayout
     *
     * @note This class implements the platform-specific details for the generic VertexBuffer interface
     */
    class OpenGLVertexBuffer : public Graphics::Renderer::VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);

        virtual ~OpenGLVertexBuffer();

        /**
         * @brief Binds this vertex buffer for rendering
         *
         * Makes this buffer active for subsequent vertex attribute operations
         * and rendering commands
         */
        void Bind() const override;

        /**
         * @brief Unbinds this vertex buffer
         *
         * Deactivates this buffer, typically used to prevent unintended modifications
         */
        void UnBind() const override;

        [[nodiscard]] const Graphics::Renderer::BufferLayout& GetLayout() const override
        {
            return m_Layout;
        }

        /**
         * @brief Sets the buffer layout
         * @param layout The BufferLayout to use for this buffer
         */
        void SetLayout(const Graphics::Renderer::BufferLayout& layout) override
        {
            m_Layout = layout;
        }

    private:
        uint32_t m_RendererID{};
        Graphics::Renderer::BufferLayout m_Layout;
    };

    // -------------------- IndexBuffer --------------------
    /**
     * @class OpenGLIndexBuffer
     * @brief OpenGL-specific implementation of the index buffer interface
     *
     * Manages index buffer objects (IBOs) for OpenGL:
     * - Stores vertex indices for indexed rendering
     * - Optimizes rendering by allowing vertex reuse
     * - Tracks the number of indices for draw calls
     *
     * @note This class implements the platform-specific details for the generic IndexBuffer interface
     */
    class OpenGLIndexBuffer : public Graphics::Renderer::IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);

        virtual ~OpenGLIndexBuffer();
        void Bind() const override;
        void UnBind() const override;

        [[nodiscard]] uint32_t GetCount() const override
        {
            return m_Count;
        };

    private:
        uint32_t m_RendererID{};
        uint32_t m_Count{};
    };
}
