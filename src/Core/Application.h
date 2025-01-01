#pragma once

#include "Core.h"

namespace AE::Core
{

    class ARNOLD_API Application
    {
    public:
        Application(const char *windowTitle) : m_Title(windowTitle) {};
        virtual ~Application();
        void Run();

        const char *m_Title;
    };

    // To be defined in client
    Application *CreateApplication();
}
