#pragma once

#include "Core/Input.h"

namespace AE::Platform::Windows
{
    /**
     * @class WindowsInput
     * @brief Windows-specific implementation of the Input interface
     *
     * Provides Windows platform-specific implementation for:
     * - Keyboard input detection
     * - Mouse button state tracking
     * - Cursor position tracking
     *
     * Uses the Win32 API for low-level input handling.
     */
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
