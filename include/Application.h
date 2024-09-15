#pragma once

#include "Window.h"

namespace AE
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();
    };

    // To be defined in client
    Application *CreateApplication();
}