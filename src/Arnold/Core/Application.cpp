#include "aepch.h"
#include "Application.h"

#include "Window.h"
#include "Arnold/Graphics/Renderer/Renderer.h"
#include "Arnold/Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"

namespace AE::Core
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
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
    }

    Application::~Application() = default;

    void Application::Run()
    {
        AE_INFO("Application running...");
        while (m_Running)
        {
            const auto time = static_cast<float>(glfwGetTime()); // TODO: Move to platform specific implementation
            const Timestep timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;

            // Update each layer
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

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
