#pragma once

#include "Application.h"
#include "Log.h"

extern AE::Core::Application *AE::Core::CreateApplication();

int main()
{
    // Move to engine initialization function
    AE::Core::Log::init();

    const auto app = AE::Core::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
