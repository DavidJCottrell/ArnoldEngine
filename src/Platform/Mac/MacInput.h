#pragma once

#include "Arnold/Core/Input.h"

namespace AE::Platform::Mac
{
    /**
     * @class MacInput
     * @brief macOS-specific implementation of the Input interface
     *
     * Provides macOS platform-specific implementation for:
     * - Keyboard input detection
     * - Mouse button state tracking
     * - Cursor position tracking
     *
     * Uses Cocoa API for low-level input handling.
     */
    class MacInput final : public Core::Input
    {
    public:
        MacInput();
        virtual ~MacInput();

    protected:
        bool IsKeyPressedImpl(int keycode) override;

        bool IsMouseButtonPressedImpl(int button) override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        std::pair<float, float> GetMousePositionImpl() override;
    };
}
