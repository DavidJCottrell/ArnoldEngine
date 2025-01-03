#pragma once

#include "Core/Input.h"

namespace AE::Platform::Mac
{
    class MacInput : public Core::Input
    {
    public:
        MacInput();
        ~MacInput();

    protected:
        bool IsKeyPressedImpl(int keycode) override;

        bool IsMouseButtonPressedImpl(int button) override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        std::pair<float, float> GetMousePositionImpl() override;
    };
}
