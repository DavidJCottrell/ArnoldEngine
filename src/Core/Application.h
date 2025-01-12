#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace AE::Core
{
    class ARNOLD_API Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();

        void OnEvent(Events::Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        [[nodiscard]] Window& GetWindow() const { return *m_Window; }

        static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(Events::WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;

        static Application* s_Instance;
    };

    // To be defined in client
    Application* CreateApplication();
}
