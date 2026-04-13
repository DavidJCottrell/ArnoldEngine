#include "aepch.h"
#include "VoxelLayer.h"

namespace AE
{
    VoxelLayer::VoxelLayer(const VoxelSceneConfig& config)
        : Core::Layer("VoxelLayer"),
          m_Scene(config)
    {}

    void VoxelLayer::OnUpdate(Core::Timestep ts)
    {
        m_Scene.Update(ts);
        m_Scene.Render();
        OnVoxelUpdate(ts);
    }

    void VoxelLayer::OnEvent(Events::Event& e)
    {
        m_Scene.OnEvent(e);
    }

    void VoxelLayer::OnImGuiRender()
    {
        m_Scene.RenderImGui();
        OnVoxelImGuiRender();
    }

    void VoxelLayer::OnKey(int keyCode, KeyCallback cb)
    {
        m_Scene.RegisterKeyCallback(keyCode, std::move(cb));
    }

    void VoxelLayer::SetPlayerController(std::shared_ptr<PlayerController> controller)
    {
        m_Scene.SetPlayerController(std::move(controller));
    }
}
