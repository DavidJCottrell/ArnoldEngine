#include "aepch.h"
#include "VoxelScene.h"
#include "PlayerController.h"

#include "Arnold/Core/MouseButtonCodes.h"
#include "Arnold/Core/KeyCodes.h"
#include "Arnold/World/WorldGenerator.h"
#include "Arnold/Graphics/Renderer/Renderer.h"
#include "Arnold/Graphics/Renderer/RenderCommand.h"
#include "Arnold/Graphics/Renderer/Buffer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace AE
{
    // ----------------------------------------------------------------
    //  Construction
    // ----------------------------------------------------------------

    VoxelScene::VoxelScene(const VoxelSceneConfig& config)
        : m_Config(config),
          m_CameraController(config.fov,
                             config.aspectRatio,
                             config.nearClip * config.blockScale,
                             config.farClip * config.blockScale)
    {
        // Fill every chunk's density field — custom generator or built-in FBM terrain
        if (config.worldGenerator)
            config.worldGenerator(m_World, config.seed);
        else
            World::WorldGenerator::GenerateTerrain(m_World, config.seed);
        m_World.SetBlockScale(config.blockScale);
        m_World.MarkAllDirty();  // trigger mesh builds on first Render()

        // Centre camera above terrain
        const float halfWorld       = static_cast<float>(World::World::WORLD_SIZE * World::Chunk::SIZE)
                                      * 0.5f * config.blockScale;
        const float terrainTopWorld = 9.0f * config.blockScale;
        const float spawnY          = terrainTopWorld + 2.0f * config.blockScale;
        m_CameraController.GetCamera().SetPosition({halfWorld, spawnY, halfWorld});
        m_CameraController.SetMoveSpeed(5.0f * config.blockScale);

        m_EditRadius = config.editRadius;

        InitMaterial();
        InitHighlightVAO();
        InitSky();
    }

    void VoxelScene::InitSky()
    {
        const auto& s = m_Config.sky;
        if (!s.enabled) return;

        auto shader = Graphics::Renderer::Shader::Create(s.skyShaderPath);
        if (!shader)
        {
            AE_CORE_ERROR("VoxelScene: sky shader failed to load — check working directory and path '{0}'", s.skyShaderPath);
            return;
        }

        m_SkyMaterial = Graphics::Renderer::Material::Create(shader);
        m_SkyMaterial->SetFloat3("u_ZenithColor",  s.zenithColor);
        m_SkyMaterial->SetFloat3("u_HorizonColor", s.horizonColor);
        m_SkyMaterial->SetFloat3("u_GroundColor",  s.groundColor);
        m_SkyMaterial->SetFloat3("u_SunDirection", glm::normalize(s.sunDirection));
        m_SkyMaterial->SetFloat3("u_SunColor",     s.sunColor);
        m_SkyMaterial->SetFloat ("u_SunSize",      s.sunSize);

        m_SkyMesh = Graphics::Renderer::Mesh::CreateCube();
    }

    void VoxelScene::InitMaterial()
    {
        auto shader = Graphics::Renderer::Shader::Create(m_Config.terrainShaderPath);
        if (!shader)
        {
            AE_CORE_ERROR("VoxelScene: terrain shader failed to load — check working directory and path '{0}'", m_Config.terrainShaderPath);
            return;
        }
        m_Material = Graphics::Renderer::Material::Create(shader);
        m_Material->SetFloat3("u_LightDir", glm::normalize(glm::vec3(0.6f, 1.0f, 0.4f)));
    }

    void VoxelScene::InitHighlightVAO()
    {
        m_HighlightShader = Graphics::Renderer::Shader::Create(m_Config.highlightShaderPath);
        if (!m_HighlightShader)
        {
            AE_CORE_ERROR("VoxelScene: highlight shader failed to load — check working directory and path '{0}'", m_Config.highlightShaderPath);
            return;
        }

        // Unit cube wireframe
        float verts[] = {
            0,0,0, 1,0,0, 1,1,0, 0,1,0,
            0,0,1, 1,0,1, 1,1,1, 0,1,1,
        };
        uint32_t idx[] = {
            0,1, 1,2, 2,3, 3,0,
            4,5, 5,6, 6,7, 7,4,
            0,4, 1,5, 2,6, 3,7,
        };

        Graphics::Renderer::BufferLayout posLayout = {
            {Graphics::Renderer::ShaderDataType::Float3, "a_Position"}
        };
        auto vb = std::shared_ptr<Graphics::Renderer::VertexBuffer>(
            Graphics::Renderer::VertexBuffer::Create(verts, sizeof(verts)));
        vb->SetLayout(posLayout);
        auto ib = std::shared_ptr<Graphics::Renderer::IndexBuffer>(
            Graphics::Renderer::IndexBuffer::Create(idx, 24));

        m_HighlightVAO.reset(Graphics::Renderer::VertexArray::Create());
        m_HighlightVAO->AddVertexBuffer(vb);
        m_HighlightVAO->SetIndexBuffer(ib);
    }

    // ----------------------------------------------------------------
    //  Frame lifecycle
    // ----------------------------------------------------------------

    void VoxelScene::Update(Core::Timestep ts)
    {
        if (m_Mode == SceneMode::Editor)
        {
            // Free-fly camera
            m_CameraController.OnUpdate(ts);

            // Raycast for edit cursor (editor only)
            const auto& cam = m_CameraController.GetCamera();
            m_RaycastResult = World::Raycast(
                m_World,
                cam.GetPosition() * (1.0f / m_Config.blockScale),
                cam.GetForward(),
                5.0f);
        }
        else
        {
            // Physics-based player — drives camera position
            if (m_PlayerController)
                m_PlayerController->Update(
                    ts,
                    m_World,
                    m_CameraController.GetCamera(),
                    m_Config.blockScale,
                    m_CameraController.IsCursorCaptured());

            m_RaycastResult = {};
        }
    }

    void VoxelScene::SetPlayerController(std::shared_ptr<PlayerController> controller)
    {
        m_PlayerController = std::move(controller);
    }

    void VoxelScene::SetMode(SceneMode mode)
    {
        if (mode == m_Mode) return;

        if (mode == SceneMode::Play)
        {
            if (!m_PlayerController)
            {
                AE_WARN("Cannot enter Play mode: no PlayerController registered. "
                        "Call SetPlayerController() before switching modes.");
                return;
            }

            // Seed the player foot at the current camera position so the view
            // doesn't jump when switching modes.
            const glm::vec3 camBlock =
                m_CameraController.GetCamera().GetPosition() * (1.0f / m_Config.blockScale);
            m_PlayerController->SetPosition(camBlock - glm::vec3(0.f, PlayerController::k_EyeHeight, 0.f));
        }

        m_Mode = mode;
    }

    void VoxelScene::RenderSky()
    {
        if (!m_SkyMaterial || !m_SkyMesh) return;

        const auto& cam = m_CameraController.GetCamera();

        // Strip translation from the view matrix so the skybox stays centred on the camera
        glm::mat4 skyVP = cam.GetProjectionMatrix()
                        * glm::mat4(glm::mat3(cam.GetViewMatrix()));
        m_SkyMaterial->SetMat4("u_ViewProjection", skyVP);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);  // sky vertices are forced to z=1.0 (far plane)

        m_SkyMaterial->Bind();
        auto& vao = m_SkyMesh->GetVertexArray();
        vao->Bind();
        Graphics::Renderer::RenderCommand::DrawIndexed(vao);
        vao->UnBind();

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    void VoxelScene::Render()
    {
        Graphics::Renderer::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Graphics::Renderer::RenderCommand::Clear();

        RenderSky();

        if (!m_Material) return;

        const auto& cam = m_CameraController.GetCamera();
        Graphics::Renderer::Renderer::BeginScene(cam);
        m_World.Render(m_Material, cam.GetViewProjectionMatrix());
        if (m_Mode == SceneMode::Editor && m_RaycastResult.hit)
            RenderHighlight();
        Graphics::Renderer::Renderer::EndScene();
    }

    void VoxelScene::RenderHighlight()
    {
        if (!m_HighlightShader || !m_HighlightVAO) return;

        const float s   = m_Config.blockScale;
        const float r   = m_EditRadius * s;
        const float eps = 0.002f * s;

        // Wireframe cube centred on the surface hit point, scaled to edit radius
        const glm::vec3 worldHit = m_RaycastResult.hitPos * s;
        glm::mat4 t = glm::translate(glm::mat4(1.0f), worldHit - glm::vec3(r * 0.5f + eps));
        t = glm::scale(t, glm::vec3(r + eps * 2.0f));

        m_HighlightShader->Bind();
        m_HighlightShader->UploadUniformMat4("u_ViewProjection",
            m_CameraController.GetCamera().GetViewProjectionMatrix());
        m_HighlightShader->UploadUniformMat4("u_Transform", t);

        glDepthMask(GL_FALSE);
        m_HighlightVAO->Bind();
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
        m_HighlightVAO->UnBind();
        glDepthMask(GL_TRUE);
    }

    void VoxelScene::RenderImGui()
    {
        // Crosshair (editor mode only)
        if (m_Mode == SceneMode::Editor)
        {
            ImDrawList* dl = ImGui::GetForegroundDrawList();
            const ImVec2 c = ImGui::GetMainViewport()->GetCenter();
            const ImU32 white = IM_COL32(255, 255, 255, 200);
            dl->AddLine({c.x - 10, c.y}, {c.x + 10, c.y}, white, 2.0f);
            dl->AddLine({c.x, c.y - 10}, {c.x, c.y + 10}, white, 2.0f);
        }

        const auto& cam = m_CameraController.GetCamera();
        ImGui::Begin("Scene");

        // ---- Header: FPS + mode ----
        ImGui::Text("%.1f FPS  (%.2f ms)", ImGui::GetIO().Framerate, 1000.f / ImGui::GetIO().Framerate);
        ImGui::Text("Mode: %s  [F5 to toggle]", m_Mode == SceneMode::Editor ? "Editor" : "Play");

        // ---- Camera ----
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Pos: (%.2f, %.2f, %.2f)",
                cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
            ImGui::Text("Yaw: %.1f  Pitch: %.1f", cam.GetYaw(), cam.GetPitch());

            float speed = m_CameraController.GetMoveSpeed();
            if (ImGui::DragFloat("Move Speed##cam", &speed, 0.1f, 0.1f, 100.f, "%.1f"))
                m_CameraController.SetMoveSpeed(speed);

            ImGui::Text("Cursor: %s  [ESC]",
                m_CameraController.IsCursorCaptured() ? "Captured" : "Normal");
        }

        // ---- Edit (editor mode) ----
        if (m_Mode == SceneMode::Editor)
        {
            if (ImGui::CollapsingHeader("Edit", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat("Radius",   &m_EditRadius,   0.1f, 0.5f, 20.f, "%.1f");
                ImGui::DragFloat("Strength", &m_EditStrength, 0.1f, 0.1f, 50.f, "%.1f");

                if (m_RaycastResult.hit)
                {
                    const glm::vec3& h = m_RaycastResult.hitPos;
                    ImGui::Text("Hit: (%.1f, %.1f, %.1f)", h.x, h.y, h.z);
                    const float d = m_World.SampleDensity(h.x, h.y, h.z);
                    ImGui::Text("Surface density: %.3f", d);
                    ImGui::TextDisabled("LMB = dig  |  RMB = fill");
                }
                else
                {
                    ImGui::Text("Hit: (none)");
                }
            }
        }

        // ---- Player (play mode) ----
        if (m_Mode == SceneMode::Play)
        {
            if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (m_PlayerController)
                {
                    const glm::vec3 p = m_PlayerController->GetPosition();
                    ImGui::Text("Foot: (%.1f, %.1f, %.1f)", p.x, p.y, p.z);
                    ImGui::Text("On Ground: %s", m_PlayerController->IsOnGround() ? "yes" : "no");

                    float ms = m_PlayerController->GetMoveSpeed();
                    if (ImGui::DragFloat("Move Speed##player", &ms, 0.1f, 0.1f, 30.f, "%.1f"))
                        m_PlayerController->SetMoveSpeed(ms);

                    float js = m_PlayerController->GetJumpSpeed();
                    if (ImGui::DragFloat("Jump Speed", &js, 0.1f, 1.f, 20.f, "%.1f"))
                        m_PlayerController->SetJumpSpeed(js);

                    float grav = m_PlayerController->GetGravity();
                    if (ImGui::DragFloat("Gravity", &grav, 0.5f, -50.f, 0.f, "%.1f"))
                        m_PlayerController->SetGravity(grav);
                }
                else
                {
                    ImGui::TextDisabled("No player registered");
                }
            }
        }

        // ---- World (collapsed by default) ----
        if (ImGui::CollapsingHeader("World"))
        {
            ImGui::Text("Block scale: %.3f", m_Config.blockScale);
            ImGui::Text("Grid: %d x %d chunks  (%d total)",
                World::World::WORLD_SIZE, World::World::WORLD_SIZE,
                World::World::WORLD_SIZE * World::World::WORLD_SIZE);
        }

        ImGui::End();
    }

    void VoxelScene::OnEvent(Events::Event& e)
    {
        // Camera controller always receives events in both modes:
        // mouse look, ESC cursor toggle, and window resize are universal.
        m_CameraController.OnEvent(e);

        Events::EventHandler handler(e);
        handler.TryHandle<Events::KeyPressedEvent>(
            [this](Events::KeyPressedEvent& ev) { return OnKeyPressed(ev); });

        // Dig / fill editing is editor-only
        if (m_Mode == SceneMode::Editor)
        {
            handler.TryHandle<Events::MouseButtonPressedEvent>(
                [this](Events::MouseButtonPressedEvent& ev) { return OnMouseButtonPressed(ev); });
        }
    }

    // ----------------------------------------------------------------
    //  Interaction handlers
    // ----------------------------------------------------------------

    bool VoxelScene::OnMouseButtonPressed(Events::MouseButtonPressedEvent& e)
    {
        if (!m_CameraController.IsCursorCaptured() || !m_RaycastResult.hit)
            return false;

        if (e.GetMouseButton() == AE_BUTTON_LEFT)
        {
            m_World.ModifyDensity(m_RaycastResult.hitPos, m_EditRadius, -m_EditStrength);
            return true;
        }

        if (e.GetMouseButton() == AE_BUTTON_RIGHT)
        {
            // Offset fill centre slightly along the surface normal so we build on top
            const glm::vec3 fillCenter = m_RaycastResult.hitPos + m_RaycastResult.normal * 0.5f;
            m_World.ModifyDensity(fillCenter, m_EditRadius, +m_EditStrength);
            return true;
        }

        return false;
    }

    bool VoxelScene::OnKeyPressed(Events::KeyPressedEvent& e)
    {
        // F5 toggles between Editor and Play mode
        if (e.GetKeyCode() == AE_KEY_F5)
        {
            SetMode(m_Mode == SceneMode::Editor ? SceneMode::Play : SceneMode::Editor);
            return true;
        }

        const auto it = m_KeyCallbacks.find(e.GetKeyCode());
        if (it != m_KeyCallbacks.end())
            it->second();
        return false;
    }

    void VoxelScene::RegisterKeyCallback(int keyCode, KeyCallback cb)
    {
        m_KeyCallbacks[keyCode] = std::move(cb);
    }
}
