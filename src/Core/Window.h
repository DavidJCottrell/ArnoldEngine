#pragma once

#define GLFW_INCLUDE_NONE
#include "Graphics/UI/ImGuiLayer.h"
#include "Events/Event.h"

namespace AE::Core
{
    struct WindowProps
    {
        std::string title;
        unsigned int width;
        unsigned int height;

        WindowProps(const std::string &title = "Arnold Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : title(title), width(width), height(height) {}
    };

    class Window
    {
    public:
        virtual ~Window();

        using EventCallbackFn = std::function<void(Events::Event &)>; // Ask claude

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual unsigned int GetWidth() const = 0;
        [[nodiscard]] virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        [[nodiscard]] virtual bool IsVSync() const = 0;

        static Window *Create(const WindowProps &props = WindowProps());

        virtual void *GetNativeWindow() const = 0;
    };
}
