#pragma once

#include "Arnold/Core/Timestep.h"
#include "Arnold/World/World.h"
#include "Arnold/Graphics/Renderer/PerspectiveCamera.h"

#include <glm/glm.hpp>

namespace AE
{
    /**
     * @brief First-person player controller with gravity and terrain collision.
     *
     * Owned by VoxelScene and active in Play mode.  Reads keyboard input,
     * integrates velocity (including gravity), resolves collisions against the
     * voxel density field, and repositions the PerspectiveCamera to the player's
     * eye height each frame.
     *
     * All positions are in **block-index space** (divide by blockScale for world space).
     */
    class PlayerController
    {
    public:
        // ----------------------------------------------------------------
        //  Tuning constants (used as default initialisers)
        // ----------------------------------------------------------------
        static constexpr float k_EyeHeight = 1.6f;   ///< Camera height above foot (blocks)
        static constexpr float k_Height     = 1.8f;   ///< Total capsule height (blocks)
        static constexpr float k_Radius     = 0.35f;  ///< Horizontal collision radius (blocks)
        static constexpr float k_MoveSpeed  = 4.5f;   ///< Horizontal speed (blocks/sec)
        static constexpr float k_JumpSpeed  = 6.0f;   ///< Initial vertical speed on jump (blocks/sec)
        static constexpr float k_Gravity    = -18.0f; ///< Gravitational acceleration (blocks/sec²)

        // ----------------------------------------------------------------
        //  Frame update
        // ----------------------------------------------------------------

        /**
         * @brief Advance physics by one timestep and update the camera.
         *
         * @param ts            Frame timestep.
         * @param world         Voxel world used for density sampling (read-only).
         * @param camera        PerspectiveCamera whose position is driven by this controller.
         * @param blockScale    World-space size of one block (used to convert block-index ↔ world).
         * @param cursorCaptured Whether the cursor is captured; movement is suppressed if false.
         */
        void Update(Core::Timestep                          ts,
                    const World::World&                     world,
                    Graphics::Renderer::PerspectiveCamera&  camera,
                    float                                   blockScale,
                    bool                                    cursorCaptured);

        // ----------------------------------------------------------------
        //  State access
        // ----------------------------------------------------------------

        /** Set foot position in block-index space.  Resets velocity. */
        void      SetPosition(const glm::vec3& blockPos);

        /** Foot position in block-index space. */
        glm::vec3 GetPosition() const { return m_Position; }

        /** True when the player is resting on solid ground. */
        bool      IsOnGround()  const { return m_OnGround; }

        // Runtime-tunable physics parameters
        float GetMoveSpeed() const  { return m_MoveSpeed; }
        void  SetMoveSpeed(float s) { m_MoveSpeed = s; }
        float GetJumpSpeed() const  { return m_JumpSpeed; }
        void  SetJumpSpeed(float s) { m_JumpSpeed = s; }
        float GetGravity()   const  { return m_Gravity; }
        void  SetGravity(float g)   { m_Gravity = g; }

    private:
        glm::vec3 m_Position{ 0.f };
        glm::vec3 m_Velocity{ 0.f };
        bool      m_OnGround = false;

        float m_MoveSpeed = k_MoveSpeed;
        float m_JumpSpeed = k_JumpSpeed;
        float m_Gravity   = k_Gravity;

        /**
         * @brief Returns true if the player capsule overlaps solid geometry at `footPos`.
         *
         * Samples the density field at a 3×3 XZ grid (offsets −r, 0, +r on each axis)
         * at both foot level (+0.1) and head level (−0.1), giving 18 sample points.
         */
        bool CollidesAt(const World::World& world, const glm::vec3& footPos) const;
    };

} // namespace AE
