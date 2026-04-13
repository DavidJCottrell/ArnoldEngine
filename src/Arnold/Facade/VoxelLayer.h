#pragma once

#include "VoxelScene.h"
#include "VoxelSceneConfig.h"
#include "Arnold/Core/Layer.h"
#include "Arnold/Core/KeyCodes.h"

namespace AE
{
    /**
     * @brief Convenience base class for voxel game layers.
     *
     * Inherit from VoxelLayer to get a fully working smooth-terrain voxel scene
     * with FPS camera, density-field editing (dig/fill), and highlight cursor.
     *
     *   - Override OnVoxelUpdate(ts) for per-frame game logic
     *   - Override OnVoxelImGuiRender() to add custom ImGui panels
     *   - Use GetWorld() / GetScene() for direct access
     *   - Call SetEditRadius(r) to change the dig/fill sphere size
     *
     * Example:
     * @code
     * class MyGame final : public AE::VoxelLayer
     * {
     * public:
     *     MyGame() : VoxelLayer({.blockScale = 0.01f, .seed = 42}) {}
     * };
     * @endcode
     */
    class VoxelLayer : public Core::Layer
    {
    public:
        explicit VoxelLayer(const VoxelSceneConfig& config = {});
        ~VoxelLayer() override = default;

        void OnUpdate(Core::Timestep ts) final;
        void OnEvent(Events::Event& e)   final;
        void OnImGuiRender()             final;

        /** Register (or replace) a key-press callback. */
        void OnKey(int keyCode, KeyCallback cb);

        /** Register the player controller created by the consumer application.
         *  Must be called before the scene can enter Play mode (F5). */
        void SetPlayerController(std::shared_ptr<PlayerController> controller);

        World::World&       GetWorld()       { return m_Scene.GetWorld(); }
        const World::World& GetWorld() const { return m_Scene.GetWorld(); }

        VoxelScene&         GetScene()       { return m_Scene; }
        const VoxelScene&   GetScene() const { return m_Scene; }

        void  SetEditRadius(float r) { m_Scene.SetEditRadius(r); }
        float GetEditRadius()  const { return m_Scene.GetEditRadius(); }

    protected:
        virtual void OnVoxelUpdate(Core::Timestep ts) {}
        virtual void OnVoxelImGuiRender() {}

    private:
        VoxelScene m_Scene;
    };
}
