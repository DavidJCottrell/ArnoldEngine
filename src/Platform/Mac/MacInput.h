#pragma once

#include "Core/Input.h"

namespace AE::Platform::Mac
{
    class MacInput : public AE::Core::Input
    {
    public:
        MacInput();
        ~MacInput();

    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;

        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
    };
}