#include "aepch.h"
#include "VoxelScene.h"

#include "Arnold/Core/MouseButtonCodes.h"
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
    }

    void VoxelScene::InitMaterial()
    {
        auto shader = Graphics::Renderer::Shader::Create(m_Config.terrainShaderPath);
        m_Material  = Graphics::Renderer::Material::Create(shader);
        m_Material->SetFloat3("u_LightDir", glm::normalize(glm::vec3(0.6f, 1.0f, 0.4f)));
    }

    void VoxelScene::InitHighlightVAO()
    {
        m_HighlightShader = Graphics::Renderer::Shader::Create(m_Config.highlightShaderPath);

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
        m_CameraController.OnUpdate(ts);

        // Raycast in block-index space (camera is in world-space, divide by blockScale)
        const auto& cam = m_CameraController.GetCamera();
        const float inv = 1.0f / m_Config.blockScale;
        m_RaycastResult = World::Raycast(
            m_World,
            cam.GetPosition() * inv,
            cam.GetForward(),
            5.0f);
    }

    void VoxelScene::Render()
    {
        Graphics::Renderer::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Graphics::Renderer::RenderCommand::Clear();

        const auto& cam = m_CameraController.GetCamera();
        Graphics::Renderer::Renderer::BeginScene(cam);
        m_World.Render(m_Material);
        if (m_RaycastResult.hit)
            RenderHighlight();
        Graphics::Renderer::Renderer::EndScene();
    }

    void VoxelScene::RenderHighlight()
    {
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
        // Crosshair
        ImDrawList* dl = ImGui::GetForegroundDrawList();
        const ImVec2 c = ImGui::GetMainViewport()->GetCenter();
        const ImU32 white = IM_COL32(255, 255, 255, 200);
        dl->AddLine({c.x - 10, c.y}, {c.x + 10, c.y}, white, 2.0f);
        dl->AddLine({c.x, c.y - 10}, {c.x, c.y + 10}, white, 2.0f);

        const auto& cam = m_CameraController.GetCamera();
        ImGui::Begin("Camera");
        ImGui::Text("Position: (%.2f, %.2f, %.2f)",
            cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
        ImGui::Text("Yaw: %.1f  Pitch: %.1f", cam.GetYaw(), cam.GetPitch());
        ImGui::Separator();
        ImGui::Text("Cursor: %s  [ESC to toggle]",
            m_CameraController.IsCursorCaptured() ? "Captured" : "Normal");
        ImGui::Separator();
        if (m_RaycastResult.hit)
        {
            ImGui::Text("Hit: (%.1f, %.1f, %.1f)",
                m_RaycastResult.hitPos.x,
                m_RaycastResult.hitPos.y,
                m_RaycastResult.hitPos.z);
            ImGui::Text("Edit radius: %.1f  [LMB=dig  RMB=fill]", m_EditRadius);
        }
        else
        {
            ImGui::Text("Hit: (none)");
        }
        ImGui::End();
    }

    void VoxelScene::OnEvent(Events::Event& e)
    {
        m_CameraController.OnEvent(e);

        Events::EventHandler handler(e);
        handler.TryHandle<Events::MouseButtonPressedEvent>(
            [this](Events::MouseButtonPressedEvent& ev) { return OnMouseButtonPressed(ev); });
        handler.TryHandle<Events::KeyPressedEvent>(
            [this](Events::KeyPressedEvent& ev) { return OnKeyPressed(ev); });
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
            m_World.ModifyDensity(m_RaycastResult.hitPos, m_EditRadius, -5.0f);
            return true;
        }

        if (e.GetMouseButton() == AE_BUTTON_RIGHT)
        {
            // Offset fill centre slightly along the surface normal so we build on top
            const glm::vec3 fillCenter = m_RaycastResult.hitPos + m_RaycastResult.normal * 0.5f;
            m_World.ModifyDensity(fillCenter, m_EditRadius, +5.0f);
            return true;
        }

        return false;
    }

    bool VoxelScene::OnKeyPressed(Events::KeyPressedEvent& e)
    {
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
