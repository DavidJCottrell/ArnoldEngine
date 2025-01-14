#include "WindowsInput.h"
#include "aepch.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"

AE::Core::Input* AE::Core::Input::s_Instance = new Platform::Windows::WindowsInput();

namespace AE::Platform::Windows
{
    WindowsInput::WindowsInput() = default;

    WindowsInput::~WindowsInput() = default;

    bool WindowsInput::IsKeyPressedImpl(const int keycode)
    {
        const auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(const int button)
    {
        const auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    float WindowsInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        const auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }
}
