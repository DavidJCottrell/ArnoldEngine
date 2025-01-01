#include "aepch.h"
#include "Application.h"
#include "Window.h"

namespace AE::Core
{
    Application::~Application() = default;

    void Application::Run()
    {
        Window window(m_Title); // Main loop
    }

}
