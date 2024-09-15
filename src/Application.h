#pragma once

#include "Window.h"
#include "Core.h"

namespace AE
{
    class ARNOLD_API Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();
    };

    // To be defined in client
    Application *CreateApplication();
}