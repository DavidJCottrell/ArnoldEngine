#pragma once

/**
 * @file
 * @brief Entry point header that provides the main() function for Arnold Engine applications
 *
 * This header defines the main() function for applications using the Arnold Engine.
 * Client applications must implement CreateApplication() to instantiate their
 * custom Application class.
 *
 * Example usage:
 * @code
 * class ExampleGame : public AE::Core::Application {
 *     // Custom game implementation
 * };
 *
 * AE::Core::Application* AE::Core::CreateApplication() {
 *     return new ExampleGame();
 * }
 * @endcode
 */

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
