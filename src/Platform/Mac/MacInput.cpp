#include "MacInput.h"
#include "aepch.h"
#include "Core/Application.h"

AE::Core::Input *AE::Core::Input::s_Instance = new Platform::Mac::MacInput();

namespace AE::Platform::Mac
{
    MacInput::MacInput() = default;

    MacInput::~MacInput() = default;

    bool MacInput::IsKeyPressedImpl(const int keycode)
    {
        const auto window = static_cast<GLFWwindow *>(AE::Core::Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    bool MacInput::IsMouseButtonPressedImpl(const int button)
    {
        const auto window = static_cast<GLFWwindow *>(AE::Core::Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }
    float MacInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }
    float MacInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }
    std::pair<float, float> MacInput::GetMousePositionImpl()
    {
        const auto window = static_cast<GLFWwindow *>(AE::Core::Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return std::pair<float, float>(xpos, ypos);
    }
}
