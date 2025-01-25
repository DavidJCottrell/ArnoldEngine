#pragma once

#include <Arnold/Graphics/Renderer/Context.h>

#include "Arnold/Core/Window.h"
#include <GLFW/glfw3.h>

using namespace AE::Core;

namespace AE::Platform::Linux
{
    /**
     * @class LinuxWindow
     * @brief Linux-specific implementation of the Window interface
     *
     * Implements platform-specific window functionality for Linux:
     * - GLFW window creation and management
     * - Native event handling
     * - OpenGL context management
     * - High DPI display support
     *
     * @note Handles Linux-specific features like Retina displays and command key shortcuts
     */
    class LinuxWindow final : public Window
    {
    public:
        explicit LinuxWindow(const WindowProps& props);
        ~LinuxWindow() override;

        void OnUpdate() override;

        [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }
        [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] void* GetNativeWindow() const override { return m_Window; };

    private:
        void Init(const WindowProps& props);
        void Shutdown() const;

        GLFWwindow* m_Window{};
        Graphics::Renderer::Context* m_Context{};

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
