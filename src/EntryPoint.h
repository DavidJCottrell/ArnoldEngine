#pragma once

extern AE::Application *AE::CreateApplication();

int main()
{
    AE::Log::init();

    auto app = AE::CreateApplication();
    app->Run();
    delete app;

    return 0;
}