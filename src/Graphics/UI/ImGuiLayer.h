#pragma once

#include "imgui_impl_glfw.h"
#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace AE::Graphics::UI
{
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
