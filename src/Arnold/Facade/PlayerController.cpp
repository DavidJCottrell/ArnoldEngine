#include "aepch.h"
#include "PlayerController.h"

#include "Arnold/Core/Input.h"
#include "Arnold/Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace AE
{
    // ----------------------------------------------------------------
    //  Collision helper
    // ----------------------------------------------------------------

    bool PlayerController::CollidesAt(const World::World& world, const glm::vec3& footPos) const
    {
        // Sample at a 3×3 XZ grid at foot level and head level (18 points total).
        // density > 0 means solid; the isosurface sits at density == 0.
        constexpr float r = k_Radius;
        constexpr float offsets[3] = { -r, 0.f, r };

        for (float dx : offsets)
        for (float dz : offsets)
        {
            if (world.SampleDensity(footPos.x + dx, footPos.y + 0.1f,              footPos.z + dz) > 0.f)
                return true;
            if (world.SampleDensity(footPos.x + dx, footPos.y + k_Height - 0.1f,  footPos.z + dz) > 0.f)
                return true;
        }
        return false;
    }

    // ----------------------------------------------------------------
    //  State
    // ----------------------------------------------------------------

    void PlayerController::SetPosition(const glm::vec3& blockPos)
    {
        m_Position = blockPos;
        m_Velocity = glm::vec3(0.f);
        m_OnGround = false;
    }

    // ----------------------------------------------------------------
    //  Frame update
    // ----------------------------------------------------------------

    void PlayerController::Update(Core::Timestep                         ts,
                                  const World::World&                    world,
                                  Graphics::Renderer::PerspectiveCamera& camera,
                                  float                                  blockScale,
                                  bool                                   cursorCaptured)
    {
        const float dt = ts.GetSeconds();

        // ---- Build horizontal move direction from keyboard ----
        glm::vec3 moveDir(0.f);
        if (cursorCaptured)
        {
            glm::vec3 fwd = camera.GetForward();
            glm::vec3 rgt = camera.GetRight();

            // Project onto XZ plane to keep movement horizontal
            glm::vec3 flatFwd(fwd.x, 0.f, fwd.z);
            glm::vec3 flatRgt(rgt.x, 0.f, rgt.z);
            if (glm::length(flatFwd) > 0.001f) flatFwd = glm::normalize(flatFwd);
            if (glm::length(flatRgt) > 0.001f) flatRgt = glm::normalize(flatRgt);

            if (Core::Input::IsKeyPressed(AE_KEY_W)) moveDir += flatFwd;
            if (Core::Input::IsKeyPressed(AE_KEY_S)) moveDir -= flatFwd;
            if (Core::Input::IsKeyPressed(AE_KEY_D)) moveDir += flatRgt;
            if (Core::Input::IsKeyPressed(AE_KEY_A)) moveDir -= flatRgt;
            if (glm::length(moveDir) > 0.f)
                moveDir = glm::normalize(moveDir);

            // ---- Jump ----
            if (Core::Input::IsKeyPressed(AE_KEY_SPACE) && m_OnGround)
            {
                m_Velocity.y = m_JumpSpeed;
                m_OnGround   = false;
            }
        }

        // ---- Apply gravity ----
        m_Velocity.y += m_Gravity * dt;

        // ---- Integrate position per-axis with collision resolution ----

        // X
        {
            glm::vec3 candidate = m_Position;
            candidate.x += moveDir.x * m_MoveSpeed * dt;
            if (!CollidesAt(world, candidate))
                m_Position.x = candidate.x;
        }

        // Z
        {
            glm::vec3 candidate = m_Position;
            candidate.z += moveDir.z * m_MoveSpeed * dt;
            if (!CollidesAt(world, candidate))
                m_Position.z = candidate.z;
        }

        // Y (gravity + jump)
        {
            glm::vec3 candidate = m_Position;
            candidate.y += m_Velocity.y * dt;
            if (!CollidesAt(world, candidate))
            {
                m_Position.y = candidate.y;
                m_OnGround   = false;
            }
            else
            {
                if (m_Velocity.y < 0.f)
                    m_OnGround = true;
                m_Velocity.y = 0.f;
            }
        }

        // ---- Drive camera to eye height ----
        camera.SetPosition((m_Position + glm::vec3(0.f, k_EyeHeight, 0.f)) * blockScale);
    }

} // namespace AE
