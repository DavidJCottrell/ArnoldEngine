#include "aepch.h"
#include "Application.h"

#include <glad/glad.h>

#include "Window.h"
#include "ConfigurationManager.h"
#include "Events/ApplicationEvent.h"

namespace AE::Core
{
    Application *Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        // Set event callback for platform-specific window
        m_Window->SetEventCallback(AE_BIND_EVENT_FN(Application::OnEvent));

        using namespace Sentinel;
        constexpr ConfigurationParameters params =
            {
                .enableHotReload = true,
                .validateOnLoad = true,
                .fileReaderType = SENTINEL};
        ConfigurationManager::Initialize("config.sen", params);
        const auto *config = ConfigurationManager::Get();

        AE_CORE_INFO("Configuration value: {0}", config->GetConfigValue());
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

    void Application::OnEvent(Events::Event &e)
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

    bool Application::OnWindowClose(Events::WindowCloseEvent &e)
    {
        AE_CORE_INFO("ArnoldEngine closing...");
        m_Running = false;
        return true;
    }
} // namespace AE::Core
