#include <gtest/gtest.h>
#include "Arnold/Events/Event.h"
#include "Arnold/Events/ApplicationEvent.h"

class EventHandlerTest : public testing::Test
{
protected:
    class TestEvent : public AE::Events::Event
    {
    public:
        static AE::Events::EventType GetStaticType() { return AE::Events::EventType::AppTick; }
        [[nodiscard]] AE::Events::EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "AppTick"; }
        [[nodiscard]] int GetCategoryFlags() const override { return AE::Events::EventCategoryApplication; }
    };
};

TEST_F(EventHandlerTest, HandleEventCorrectType)
{
    TestEvent event;
    AE::Events::EventHandler handler(event);
    bool handlerCalled = false;

    const bool result = handler.TryHandle<TestEvent>([&handlerCalled](TestEvent& e)
    {
        handlerCalled = true;
        return true;
    });

    EXPECT_TRUE(result);
    EXPECT_TRUE(handlerCalled);
}

TEST_F(EventHandlerTest, HandleEventWrongType)
{
    TestEvent event;
    AE::Events::EventHandler handler(event);
    bool handlerCalled = false;

    bool result = handler.TryHandle<AE::Events::WindowResizeEvent>([&handlerCalled](AE::Events::WindowResizeEvent& e)
    {
        handlerCalled = true;
        return true;
    });

    EXPECT_FALSE(result);
    EXPECT_FALSE(handlerCalled);
}

TEST_F(EventHandlerTest, EventHandledFlagIsSet)
{
    TestEvent event;
    AE::Events::EventHandler handler(event);

    handler.TryHandle<TestEvent>([](TestEvent& e)
    {
        return true; // Mark as handled
    });

    EXPECT_TRUE(event.Handled);
}

TEST_F(EventHandlerTest, EventHandledFlagNotSet)
{
    TestEvent event;
    AE::Events::EventHandler handler(event);

    handler.TryHandle<TestEvent>([](TestEvent& e)
    {
        return false; // Mark as not handled
    });

    EXPECT_FALSE(event.Handled);
}

TEST_F(EventHandlerTest, MultipleHandlers)
{
    TestEvent event;
    AE::Events::EventHandler handler(event);
    int handlerCallCount = 0;

    handler.TryHandle<TestEvent>([&handlerCallCount](TestEvent& e)
    {
        handlerCallCount++;
        return false; // Don't mark as handled
    });

    handler.TryHandle<TestEvent>([&handlerCallCount](TestEvent& e)
    {
        handlerCallCount++;
        return true;
    });

    EXPECT_EQ(handlerCallCount, 2);
    EXPECT_TRUE(event.Handled);
}
