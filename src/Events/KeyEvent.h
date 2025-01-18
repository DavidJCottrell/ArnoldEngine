#pragma once

#include "Event.h"

namespace AE::Events
{
    /**
     * @class KeyEvent
     * @brief Base class for keyboard input events
     *
     * Abstract base class that provides common functionality for all keyboard-related events.
     * Stores and provides access to the keycode of the affected key.
     */
    class ARNOLD_API KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(int keyCode)
            : m_KeyCode(keyCode)
        {
        }

        int m_KeyCode;
    };

    /**
     * @class KeyPressedEvent
     * @brief Event triggered when a keyboard key is pressed
     *
     * Represents a key press event, providing information about which key was pressed.
     * Inherits from KeyEvent to handle keyboard input processing.
     */
    class ARNOLD_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keyCode) : KeyEvent(keyCode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    };

    /**
     * @class KeyReleasedEvent
     * @brief Event triggered when a keyboard key is released
     *
     * Represents a key release event, providing information about which key was released.
     * Used for handling the end of key press actions.
     */
    class ARNOLD_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode)
            : KeyEvent(keyCode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    /**
     * @class KeyTypedEvent
     * @brief Event triggered when a key is typed (pressed and released)
     *
     * Represents a complete key press and release cycle, useful for text input.
     * Provides the character value of the typed key.
     */
    class ARNOLD_API KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode) : KeyEvent(keyCode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
