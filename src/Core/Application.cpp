#include "aepch.h"
#include "Application.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace AE::Core
{

    Application *Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(AE_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application() = default;

    void Application::Run()
    {
        AE_INFO("Application running...");
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            // Update each layer
            for (Layer *layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(AE::Events::Event &e)
    {
        AE::Events::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<AE::Events::WindowCloseEvent>(AE_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e); // Call OnEvent for each layer until it has been handled / consumed
            if (e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(AE::Events::WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

} // namespace AE::Core
