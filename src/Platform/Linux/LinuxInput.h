#pragma once

#include "Arnold/Core/Input.h"

namespace AE::Platform::Linux
{
    /**
     * @class LinuxInput
     * @brief Linux-specific implementation of the Input interface
     *
     * Provides Linux platform-specific implementation for:
     * - Keyboard input detection
     * - Mouse button state tracking
     * - Cursor position tracking
     *
     * Uses X11/Wayland for low-level input handling.
     */
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
