#include "aepch.h"
#include "Application.h"

#include <glad/glad.h>

#include <memory>

#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace AE::Core
{
    Application* Application::s_Instance = nullptr;

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

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());

        // Set event callback for platform-specific window
        m_Window->SetEventCallback(AE_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new Graphics::UI::ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        constexpr float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.7f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.7f, 1.0f, 1.0f
        };

        m_VertexBuffer.reset(Graphics::Renderer::VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            const Graphics::Renderer::BufferLayout layout = {
                {Graphics::Renderer::ShaderDataType::Float3, "a_Position"},
                {Graphics::Renderer::ShaderDataType::Float4, "a_Color"},
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
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


        uint32_t indices[3] = {0, 1, 2};
        m_IndexBuffer.reset(Graphics::Renderer::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader = std::make_unique<Graphics::Shader>(vertexSrc, fragmentSrc);
    }

    Application::~Application() = default;

    void Application::Run()
    {
        AE_INFO("Application running...");
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            // Update each layer
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            // Render ImGui UI
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Events::Event& e)
    {
        Events::EventHandler handler(e);

        handler.TryHandle<Events::WindowCloseEvent>(AE_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e); // Pass event to each layer until it has been handled / consumed
            if (e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(Events::WindowCloseEvent& e)
    {
        AE_CORE_INFO("ArnoldEngine closing...");
        m_Running = false;
        return true;
    }
}
