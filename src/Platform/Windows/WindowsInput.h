#pragma once

#include "Core/Input.h"

namespace AE::Platform::Windows
{
    class WindowsInput : public Core::Input
    {
    public:
        WindowsInput();
        virtual ~WindowsInput();

    protected:
        bool IsKeyPressedImpl(int keycode) override;

        bool IsMouseButtonPressedImpl(int button) override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        std::pair<float, float> GetMousePositionImpl() override;
    };
}
