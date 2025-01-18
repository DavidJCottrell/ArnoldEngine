#pragma once

#include "Event.h"
#include <string>
#include <sstream>

namespace AE::Events
{
    /**
     * @class MouseMovedEvent
     * @brief Event triggered when the mouse cursor moves
     *
     * Tracks the current position of the mouse cursor.
     * Provides X and Y coordinates of the cursor's current position.
     */
    class ARNOLD_API MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y)
        {
        }

        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_MouseX, m_MouseY;
    };

    /**
     * @class MouseScrolledEvent
     * @brief Event triggered when the mouse wheel is scrolled
     *
     * Tracks both vertical and horizontal scroll movements.
     * Provides offset values for both X and Y scroll directions.
     */
    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset)
        {
        }

        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_XOffset, m_YOffset;
    };

    /**
     * @class MouseButtonEvent
     * @brief Base class for mouse button events
     *
     * Abstract base class that provides common functionality for all mouse button events.
     * Stores and provides access to the button code of the affected mouse button.
     */
    class MouseButtonEvent : public Event
    {
    public:
        inline int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

    protected:
        MouseButtonEvent(int button)
            : m_Button(button)
        {
        }

        int m_Button;
    };

    /**
     * @class MouseButtonPressedEvent
     * @brief Event triggered when a mouse button is pressed
     *
     * Represents a mouse button press event.
     * Provides information about which button was pressed.
     */
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    /**
     * @class MouseButtonReleasedEvent
     * @brief Event triggered when a mouse button is released
     *
     * Represents a mouse button release event.
     * Provides information about which button was released.
     */
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}
