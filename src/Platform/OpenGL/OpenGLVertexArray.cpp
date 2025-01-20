#include "aepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace AE::Platform::OpenGL
{
    static GLenum ShaderDataTypeToOpenGLBaseType(Graphics::Renderer::ShaderDataType type)
    {
        switch (type)
        {
        case Graphics::Renderer::ShaderDataType::Float: return GL_FLOAT;
        case Graphics::Renderer::ShaderDataType::Float2: return GL_FLOAT;
        case Graphics::Renderer::ShaderDataType::Float3: return GL_FLOAT;
        case Graphics::Renderer::ShaderDataType::Float4: return GL_FLOAT;
        case Graphics::Renderer::ShaderDataType::Mat3: return GL_FLOAT;
        case Graphics::Renderer::ShaderDataType::Mat4: return GL_FLOAT;
        case Graphics::Renderer::ShaderDataType::Int: return GL_INT;
        case Graphics::Renderer::ShaderDataType::Int2: return GL_INT;
        case Graphics::Renderer::ShaderDataType::Int3: return GL_INT;
        case Graphics::Renderer::ShaderDataType::Int4: return GL_INT;
        case Graphics::Renderer::ShaderDataType::Bool: return GL_BOOL;
        default:
            AE_CORE_ERROR("Unknown shader type");
            return 0;
        }
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_RendererId);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererId);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererId);
    }

    void OpenGLVertexArray::UnBind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<Graphics::Renderer::VertexBuffer>& vertexBuffer)
    {
        const auto& layout = vertexBuffer->GetLayout();
        if (layout.GetElements().empty())
        {
            AE_CORE_ERROR("Vertex buffer has no layout");
            return;
        }

        glBindVertexArray(m_RendererId);
        vertexBuffer->Bind();

        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.GetComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                  element.Normalised ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  reinterpret_cast<const void*>(element.Offset));
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<Graphics::Renderer::IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererId);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
