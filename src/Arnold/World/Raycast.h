#pragma once

#include <glm/glm.hpp>

namespace AE::World
{
    class World;

    struct RaycastResult
    {
        bool      hit    = false;
        glm::vec3 hitPos = {};   // block-index-space hit point (before blockScale)
        glm::vec3 normal = {};   // outward surface normal
    };

    /** Cast a ray through the world's density field and return the first surface crossing.
     *  @param origin     Ray origin in block-index space.
     *  @param direction  Ray direction (need not be normalised).
     *  @param maxDistance  Maximum travel distance in blocks. */
    RaycastResult Raycast(const World& world,
                          const glm::vec3& origin,
                          const glm::vec3& direction,
                          float maxDistance = 5.0f);
}
