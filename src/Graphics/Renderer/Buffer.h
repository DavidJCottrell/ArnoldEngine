#pragma once

#include <cstdint>
#include "Core/Log.h"

namespace AE::Graphics::Renderer
{
    /**
     * @enum ShaderDataType
     * @brief Defines the supported data types for shader attributes
     *
     * Enumerates the basic and composite data types that can be used
     * in shader vertex attributes:
     * - Basic types (Float, Int, Bool)
     * - Vector types (Float2, Float3, Float4, etc.)
     * - Matrix types (Mat3, Mat4)
     */
    enum class ShaderDataType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float: return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;
        case ShaderDataType::Mat3: return 4 * 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        case ShaderDataType::Int: return 4;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;
        case ShaderDataType::Bool: return 1;
        default:
            AE_CORE_ERROR("Unknown ShaderDataType!");
            return 0;
        }
    }

    /**
     * @struct BufferElement
     * @brief Describes a single element in a vertex buffer layout
     *
     * Contains all the information needed to describe how a single vertex attribute
     * is laid out in memory:
     * - Type and size information
     * - Memory offset within vertex
     * - Normalization settings
     * - Name for shader mapping
     */
    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalised;

        BufferElement() = default;

        BufferElement(const ShaderDataType type, std::string name, const bool normalised = false)
            : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalised(normalised)
        {
        }

        [[nodiscard]] uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            default:
                AE_CORE_ERROR("Unknown ShaderDataType!");
                return 0;
            }
        }
    };

    /**
     * @class BufferLayout
     * @brief Manages the layout of vertex attributes in a GPU buffer
     *
     * The BufferLayout class provides a way to describe how vertex data is organized in memory,
     * enabling proper interpretation of vertex buffer data by the graphics pipeline. It:
     * - Tracks the size and offset of each vertex attribute
     * - Calculates stride between vertices
     * - Supports iteration over buffer elements
     *
     * Example usage:
     * @code
     * BufferLayout layout = {
     *     {ShaderDataType::Float3, "a_Position"},
     *     {ShaderDataType::Float4, "a_Color"}
     * };
     * vertexBuffer->SetLayout(layout);
     * @endcode
     */
    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        };

        /**
         * @brief Gets the stride between vertices in bytes
         * @return Total size of one vertex in bytes
         *
         * The stride is the total size of all elements in the layout,
         * representing the distance in bytes between consecutive vertices.
         */
        [[nodiscard]] uint32_t GetStride() const
        {
            return m_Stride;
        }

        /**
         * @brief Gets the list of buffer elements
         * @return Const reference to the vector of buffer elements
         */
        [[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_Elements; }


        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        /**
         * @brief Calculates offsets for each element and total stride
         *
         * This method:
         * - Computes the byte offset of each element within a vertex
         * - Updates the total stride (size of one complete vertex)
         * - Must be called whenever the layout elements change
         */
        void CalculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

        std::vector<BufferElement> m_Elements;;
        uint32_t m_Stride{};
    };

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

        [[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& BufferLayout) = 0;

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

        static IndexBuffer* Create(const uint32_t* indices, uint32_t size);
    };
}



