#pragma once

#include "Core/Core.h"
#include "aepch.h"

namespace AE::Events
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type)                                          \
    static EventType GetStaticType() { return EventType::type; }        \
    EventType GetEventType() const override { return GetStaticType(); } \
    const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    int GetCategoryFlags() const override { return category; }

    class ARNOLD_API Event
    {
        friend class EventHandler;

    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

        bool Handled = false;
    };

    /**
     * @brief A utility class that safely routes events to type-specific handler functions.
     *
     * The EventHandler provides a type-safe way to handle different types of events by:
     * 1. Checking if an incoming event matches a specific type
     * 2. If matched, converting the event to its specific type safely
     * 3. Routing it to the appropriate handler function
     *
     * Example usage:
     * @code
     * void OnEvent(Event& e) {
     *     EventHandler handler(e);
     *
     *     // Try to handle a window close event
     *     if (handler.TryHandle<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose))) {
     *         return; // Event was successfully handled
     *     }
     *
     *     // Event wasn't a window close event, try other handlers...
     * }
     *
     * bool OnWindowClose(WindowCloseEvent& e) {
     *     // Handle window close...
     *     return true;  // Mark event as handled
     * }
     * @endcode
     *
     * @note Handler functions should return true if they've handled the event and false
     * if the event should continue propagating to other handlers.
     */
    class EventHandler
    {
        // Define a template alias for our event handler function type
        template <typename T>
        using HandlerFn = std::function<bool(T&)>;

    public:
        /**
         * @brief Constructs an EventHandler for a specific event.
         * @param event The event to potentially handle.
         */
        EventHandler(Event& event)
            : m_Event(event)
        {
        }

        /**
         * @brief Attempts to handle an event if it matches the specified type.
         *
         * @tparam T The specific event type to check for.
         * @param handler The function to handle the event if types match.
         * @return true if the event type matched and the handler was called,
         *         false if the event was of a different type.
         */
        template <typename T>
        bool TryHandle(HandlerFn<T> handler)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = handler(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event; // Reference to the event being handled
    };

    inline std::string format_as(const Event& e)
    {
        return e.ToString();
    }
}
