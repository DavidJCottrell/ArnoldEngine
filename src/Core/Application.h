#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Graphics/Renderer/Shader.h"

namespace AE::Core
{
    /**
     * @class Application
     * @brief The core application class that manages the game/application lifecycle
     *
     * The Application class is the central class of the Arnold Engine. It:
     * - Manages the main window
     * - Handles the main game loop
     * - Manages layers and overlays
     * - Processes events
     *
     * There should only be one Application instance per program, managed through
     * the singleton pattern with Application::Get().
     *
     * @note Client applications must implement CreateApplication() to instantiate
     * their custom Application class.
     */
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
        Graphics::UI::ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        static Application* s_Instance;

        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
        std::unique_ptr<Graphics::Shader> m_Shader;
    };

    // To be defined in client
    Application* CreateApplication();
}
