#pragma once

#include "Core/Input.h"

namespace AE::Platform::Linux
{
    class LinuxInput final : public Core::Input
    {
    public:
        LinuxInput();
        virtual ~LinuxInput();

    protected:
        bool IsKeyPressedImpl(int keycode) override;

        bool IsMouseButtonPressedImpl(int button) override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        std::pair<float, float> GetMousePositionImpl() override;
    };
}
