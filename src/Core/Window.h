#pragma once

/**
 * @file
 * @brief Abstract window interface for platform-specific window implementations
 */

#define GLFW_INCLUDE_NONE
#include "Graphics/UI/ImGuiLayer.h"
#include "Events/Event.h"

namespace AE::Core
{
    /**
     * @brief Properties for initializing a window
     */
    struct WindowProps
    {
        std::string title;
        unsigned int width;
        unsigned int height;

        /**
         * @brief Constructs window properties with default or specified values
         * @param title Window title (defaults to "Arnold Engine")
         * @param width Window width in pixels (defaults to 1280)
         * @param height Window height in pixels (defaults to 720)
         */
        WindowProps(const std::string& title = "Arnold Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : title(title), width(width), height(height)
        {
        }
    };

    /**
     * @brief Abstract base class for platform-specific window implementations
     *
     * The Window class provides a common interface for creating and managing windows
     * across different platforms. Platform-specific implementations (e.g., MacWindow,
     * WindowsWindow) inherit from this class and implement its pure virtual methods.
     */
    class Window
    {
    public:
        virtual ~Window();

        using EventCallbackFn = std::function<void(Events::Event&)>; // Ask claude

        /**
         * @brief Updates the window (processes events, swaps buffers)
         */
        virtual void OnUpdate() = 0;

        /**
        * @brief Gets the window width
        * @return Current window width in pixels
        */
        [[nodiscard]] virtual unsigned int GetWidth() const = 0;

        /**
         * @brief Gets the window height
         * @return Current window height in pixels
         */
        [[nodiscard]] virtual unsigned int GetHeight() const = 0;

        /**
         * @brief Sets the event callback function for window events
         * @param callback Function to handle window events
         */
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        /**
         * @brief Enables or disables vertical synchronization
         * @param enabled True to enable VSync, false to disable
         */
        virtual void SetVSync(bool enabled) = 0;

        /**
         * @brief Checks if VSync is currently enabled
         * @return True if VSync is enabled, false otherwise
         */
        [[nodiscard]] virtual bool IsVSync() const = 0;

        /**
         * @brief Creates a platform-specific window instance
         * @param props Window properties for initialization
         * @return Pointer to created window instance
         */
        static Window* Create(const WindowProps& props = WindowProps());

        /**
         * @brief Gets the native window handle
         * @return Pointer to the platform-specific window handle
         */
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;

        static void GetWindowProperties(unsigned int* width, unsigned int* height,
                                        float* xScale, float* yScale);
    };
}
