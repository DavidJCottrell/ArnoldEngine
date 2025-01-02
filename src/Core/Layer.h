#pragma once

#include "Events/Event.h"

namespace AE::Core
{
    class ARNOLD_API Layer
    {
    public:
        Layer(const std::string &debugName = "Layer");
        ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(AE::Events::Event &event) {}

        inline const std::string &GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}