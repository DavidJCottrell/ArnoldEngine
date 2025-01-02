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

        void OnEvent(AE::Events::Event &e);

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *overlay);

        Window &GetWindow() { return *m_Window; }

        inline static Application &Get() { return *s_Instance; }

        const char *m_Title;

    private:
        bool OnWindowClose(AE::Events::WindowCloseEvent &e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;

        static Application *s_Instance;
    };

    // To be defined in client
    Application *CreateApplication();
}
