#pragma once

#include "Core/Window.h"
#include <GLFW/glfw3.h>

using namespace AE::Core;

namespace AE::Platform::Mac
{
    class MacWindow : public Window
    {
    public:
        MacWindow(const WindowProps& props);
        ~MacWindow();

        void OnUpdate() override;

        [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }
        [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] void* GetNativeWindow() const override { return m_Window; };

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

        GLFWwindow* m_Window{};

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
