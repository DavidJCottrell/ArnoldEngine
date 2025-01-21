#include "aepch.h"
#include "Application.h"

#include <Arnold/Events/KeyEvent.h>

#include "KeyCodes.h"
#include "Arnold/Graphics/Renderer/Renderer.h"

#include "Window.h"
#include "Arnold/Events/ApplicationEvent.h"

namespace AE::Core
{
    Application* Application::s_Instance = nullptr;

    Application::Application() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        if (s_Instance != nullptr)
        {
            throw std::runtime_error("Application already initialized");
        }
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());

        // Set event callback for platform-specific window
        m_Window->SetEventCallback(AE_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new Graphics::UI::ImGuiLayer();
        PushOverlay(m_ImGuiLayer);


        // ----------- TRIANGLE -----------
        m_TriangleVertexArray.reset(Graphics::Renderer::VertexArray::Create());

        constexpr float triangleVertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.7f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.7f, 1.0f, 1.0f
        };

        std::shared_ptr<Graphics::Renderer::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(Graphics::Renderer::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
        const Graphics::Renderer::BufferLayout layout = {
            {Graphics::Renderer::ShaderDataType::Float3, "a_Position"},
            {Graphics::Renderer::ShaderDataType::Float4, "a_Color"},
        };
        squareVertexBuffer->SetLayout(layout);
        m_TriangleVertexArray->AddVertexBuffer(squareVertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Graphics::Renderer::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(Graphics::Renderer::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_TriangleVertexArray->SetIndexBuffer(squareIndexBuffer);


        // ----------- SQUARE -----------
        m_SquareVertexArray.reset(Graphics::Renderer::VertexArray::Create());

        constexpr float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f, 0.75f, 0.0f,
            -0.75f, 0.75f, 0.0f
        };

        // Create the vertex buffer
        std::shared_ptr<Graphics::Renderer::VertexBuffer> squareVB;
        squareVB.reset(Graphics::Renderer::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        // Set the layout of the vertex buffer
        squareVB->SetLayout({
            {Graphics::Renderer::ShaderDataType::Float3, "a_Position"},
        });
        // Add the vertex buffer to the vertex array
        m_SquareVertexArray->AddVertexBuffer(squareVB);


        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        // Create the index buffer
        std::shared_ptr<Graphics::Renderer::IndexBuffer> squareIB;
        squareIB.reset(
            Graphics::Renderer::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

        // Add the index buffer to the vertex array
        m_SquareVertexArray->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_TriangleShader = std::make_unique<Graphics::Renderer::Shader>(vertexSrc, fragmentSrc);

        std::string vertexSrc2 = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc2 = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_SquareShader = std::make_unique<Graphics::Renderer::Shader>(vertexSrc2, fragmentSrc2);
    }

    Application::~Application() = default;

    void Application::Run()
    {
        AE_INFO("Application running...");
        while (m_Running)
        {
            Graphics::Renderer::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            Graphics::Renderer::RenderCommand::Clear();

            Graphics::Renderer::Renderer::BeginScene(m_Camera);

            Graphics::Renderer::Renderer::Submit(m_SquareShader, m_SquareVertexArray);
            Graphics::Renderer::Renderer::Submit(m_TriangleShader, m_TriangleVertexArray);

            Graphics::Renderer::Renderer::EndScene();

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

        handler.TryHandle<Events::KeyPressedEvent>(AE_BIND_EVENT_FN(Application::OnKeyEvent));

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

    bool Application::OnKeyEvent(Events::KeyEvent& e)
    {
        glm::vec3 newPosition = m_Camera.GetPosition();
        switch (e.GetKeyCode())
        {
        case AE_KEY_W:
            newPosition.y -= 0.5f;
            m_Camera.SetPosition(newPosition);
            break;
        case AE_KEY_S:
            newPosition.y += 0.5f;
            m_Camera.SetPosition(newPosition);
            break;
        case AE_KEY_A:
            newPosition.x += 0.5f;
            m_Camera.SetPosition(newPosition);
            break;
        case AE_KEY_D:
            newPosition.x -= 0.5f;
            m_Camera.SetPosition(newPosition);
            break;
        default: break;
        }

        return false;
    }
}
