#pragma once

#include "Events/Event.h"

namespace AE::Core
{
    class Layer
    {
    public:
        Layer();
        ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event &event) {}

    private:
    };
}