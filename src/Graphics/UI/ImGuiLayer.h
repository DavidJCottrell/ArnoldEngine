#pragma once

#include "imgui_impl_glfw.h"
#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace AE::Graphics::UI
{
    /**
     * @class ImGuiLayer
     * @brief Manages Dear ImGui integration for debugging and tools
     *
     * The ImGuiLayer provides immediate mode GUI functionality:
     * - Debug windows and tools
     * - Real-time property editing
     * - Performance monitoring
     * - Custom editor tools
     *
     * @note This layer typically runs as an overlay to render on top of other layers
     */
    class ARNOLD_API ImGuiLayer : public Core::Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        /** Allow Layers to do their own ImGui rendering */
        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
        static ImGuiKey MapSpecialKeys(int keyCode);
    };
}
