#pragma once

#include "Arnold/Events/Event.h"

namespace AE::Core
{
    /**
     * @class Layer
     * @brief Base class for application layers that can be pushed/popped from the LayerStack
     *
     * Layers are a fundamental part of the engine's architecture, allowing for:
     * - Modular organization of game/application logic
     * - Independent update and render loops
     * - Event handling with propagation control
     * - ImGui integration for debugging/tools
     *
     * Layers are processed in order, with later layers potentially overriding earlier ones.
     * Each layer can handle events and choose whether to pass them to layers below.
     */
    class ARNOLD_API Layer
    {
    public:
        explicit Layer(const std::string& debugName = "Layer");
        virtual ~Layer();

        virtual void OnAttach()
        {
        }

        virtual void OnDetach()
        {
        }

        virtual void OnUpdate()
        {
        }

        virtual void OnImGuiRender()
        {
        }

        virtual void OnEvent(Events::Event& event)
        {
        }

        [[nodiscard]] const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
