#pragma once

#include "Event.h"
#include <string>
#include <sstream>

namespace AE::Events
{
    /**
     * @class WindowResizeEvent
     * @brief Event triggered when the window is resized
     *
     * Provides information about the new dimensions of the window after a resize event.
     * Includes the new width and height of the window.
     */
    class ARNOLD_API WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(const unsigned int width, const unsigned int height)
            : m_Width(width), m_Height(height)
        {
        }

        [[nodiscard]] unsigned int GetWidth() const { return m_Width; }
        [[nodiscard]] unsigned int GetHeight() const { return m_Height; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        unsigned int m_Width, m_Height;
    };

    /**
     * @class WindowCloseEvent
     * @brief Event triggered when the window is closed
     *
     * Simple event class that signals when the application window is being closed.
     * Used to properly handle application shutdown.
     */
    class ARNOLD_API WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @class AppTickEvent
     * @brief Event triggered on each tick of the application
     *
     * Represents a single update tick in the application's main loop.
     * Used for timing-based updates and game logic.
     */
    class ARNOLD_API AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };


    /**
     * @class AppUpdateEvent
     * @brief Event triggered during the application update phase
     *
     * Signals the start of an update cycle in the application.
     * Used to coordinate update timing across different systems.
     */
    class ARNOLD_API AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @class AppRenderEvent
     * @brief Event triggered during the render phase
     *
     * Signals when the application should perform rendering operations.
     * Used to coordinate rendering timing across different systems.
     */
    class ARNOLD_API AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}
