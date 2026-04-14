#pragma once

#include "VoxelSceneConfig.h"
#include "PlayerController.h"

#include "Arnold/Core/Timestep.h"
#include "Arnold/Events/Event.h"
#include "Arnold/Events/MouseEvent.h"
#include "Arnold/Events/KeyEvent.h"
#include "Arnold/World/World.h"
#include "Arnold/World/Raycast.h"
#include "Arnold/Graphics/Renderer/FpsCameraController.h"
#include "Arnold/Graphics/Renderer/Material.h"
#include "Arnold/Graphics/Renderer/Mesh.h"
#include "Arnold/Graphics/Renderer/Shader.h"
#include "Arnold/Graphics/Renderer/TextureAtlas.h"
#include "Arnold/Graphics/Renderer/VertexArray.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace AE
{
    /** Callback invoked when a registered key is pressed. */
    using KeyCallback = std::function<void()>;

    /** Controls whether the scene is in free-fly editor mode or physics-based play mode. */
    enum class SceneMode { Editor, Play };

    /**
     * @brief Owns all runtime objects for a voxel scene.
     *
     * VoxelScene bundles the World, FPS camera, material/shader pipeline,
     * per-frame density raycasting, and sphere-based dig/fill editing.
     * It is NOT a Layer. VoxelLayer owns a VoxelScene and delegates to it.
     */
    class VoxelScene
    {
    public:
        explicit VoxelScene(const VoxelSceneConfig& config = {});

        VoxelScene(const VoxelScene&)            = delete;
        VoxelScene& operator=(const VoxelScene&) = delete;

        // ----------------------------------------------------------------
        //  Frame lifecycle
        // ----------------------------------------------------------------

        void Update(Core::Timestep ts);
        void Render();
        void RenderImGui();
        void OnEvent(Events::Event& e);

        // ----------------------------------------------------------------
        //  User-extensibility
        // ----------------------------------------------------------------

        void RegisterKeyCallback(int keyCode, KeyCallback cb);

        // ----------------------------------------------------------------
        //  Accessors
        // ----------------------------------------------------------------

        World::World&       GetWorld()       { return m_World; }
        const World::World& GetWorld() const { return m_World; }

        Graphics::Renderer::FpsCameraController&       GetCamera()       { return m_CameraController; }
        const Graphics::Renderer::FpsCameraController& GetCamera() const { return m_CameraController; }

        float GetBlockScale() const { return m_Config.blockScale; }

        void  SetEditRadius(float r) { m_EditRadius = r; }
        float GetEditRadius()  const { return m_EditRadius; }

        const World::RaycastResult& GetLastRaycast() const { return m_RaycastResult; }

        // ----------------------------------------------------------------
        //  Mode (Editor / Play)
        // ----------------------------------------------------------------

        SceneMode GetMode()        const { return m_Mode; }

        /** Switch between editor free-fly and physics-based play.
         *  Refuses to enter Play mode if no PlayerController has been registered. */
        void SetMode(SceneMode mode);

        /** Register a player controller created by the consumer application.
         *  Must be called before the scene can enter Play mode. */
        void SetPlayerController(std::shared_ptr<PlayerController> controller);

        PlayerController*       GetPlayerController()       { return m_PlayerController.get(); }
        const PlayerController* GetPlayerController() const { return m_PlayerController.get(); }

    private:
        void InitMaterial();
        void InitHighlightVAO();
        void InitSky();
        void RenderSky();
        void RenderHighlight();
        bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& e);
        bool OnKeyPressed(Events::KeyPressedEvent& e);

        VoxelSceneConfig                                  m_Config;
        World::World                                      m_World;
        Graphics::Renderer::FpsCameraController           m_CameraController;
        std::shared_ptr<Graphics::Renderer::Material>     m_Material;
        std::shared_ptr<Graphics::Renderer::TextureAtlas> m_TerrainAtlas;
        std::shared_ptr<Graphics::Renderer::Material>     m_SkyMaterial;
        std::unique_ptr<Graphics::Renderer::VertexArray>  m_SkyVAO;
        std::shared_ptr<Graphics::Renderer::Shader>       m_HighlightShader;
        std::unique_ptr<Graphics::Renderer::VertexArray>  m_HighlightVAO;

        World::RaycastResult                              m_RaycastResult;
        float                                             m_EditRadius   = 2.0f;
        float                                             m_EditStrength = 5.0f; ///< Density delta per dig/fill click

        SceneMode                         m_Mode = SceneMode::Editor;
        std::shared_ptr<PlayerController> m_PlayerController;  ///< Null until registered by the sandbox

        std::unordered_map<int, KeyCallback> m_KeyCallbacks;
    };
}
