#pragma once

#include "Application.h"
#include "Log.h"

extern AE::Application *AE::CreateApplication();

int main()
{
    // Move to engine initialization function
    AE::Log::init();

    auto app = AE::CreateApplication();
    app->Run();
    delete app;

    return 0;
}