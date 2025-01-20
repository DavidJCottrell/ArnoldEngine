#include "Arnold/Core/Window.h"
#include "Application.h"
#include "Platform/Mac/MacWindow.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"

namespace AE::Core
{
    Window::~Window() = default;

    Window* Window::Create(const WindowProps& props)
    {
#if defined(__APPLE__)
        return new Platform::Mac::MacWindow(props);
#elif defined(_WIN32)
        return new Platform::Windows::WindowsWindow(props);
#elif defined(__linux__)
        return new Platform::Linux::LinuxWindow(props);
#else
        throw std::runtime_error("Linux platform not supported yet!");
        #error "Platform not supported!"
#endif
    }
}
