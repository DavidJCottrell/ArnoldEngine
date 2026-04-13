#pragma once

#include <array>
#include <glm/glm.hpp>

namespace AE::World::Frustum
{
    /**
     * @brief Extract 6 normalised frustum planes from a view-projection matrix.
     *
     * Uses the Gribb/Hartmann method (row combination on the VP matrix).
     * Each plane is a vec4 where xyz is the inward-pointing normal and w is the
     * signed distance from the origin.  A point P is inside the frustum if
     *   dot(plane.xyz, P) + plane.w >= 0
     * for all six planes.
     */
    inline std::array<glm::vec4, 6> ExtractPlanes(const glm::mat4& m)
    {
        // GLM is column-major: m[col][row].
        // Row i = { m[0][i], m[1][i], m[2][i], m[3][i] }
        auto row = [&](int i) -> glm::vec4
        {
            return { m[0][i], m[1][i], m[2][i], m[3][i] };
        };

        std::array<glm::vec4, 6> planes;
        planes[0] = row(3) + row(0);  // left
        planes[1] = row(3) - row(0);  // right
        planes[2] = row(3) + row(1);  // bottom
        planes[3] = row(3) - row(1);  // top
        planes[4] = row(3) + row(2);  // near
        planes[5] = row(3) - row(2);  // far

        for (auto& p : planes)
        {
            const float len = glm::length(glm::vec3(p));
            if (len > 1e-6f) p /= len;
        }
        return planes;
    }

    /**
     * @brief Returns true if an AABB is at least partially inside the frustum.
     *
     * Uses the "positive vertex" test: for each plane, the corner of the box
     * most aligned with the plane normal is tested.  If that corner is outside,
     * the whole box is outside that plane and can be rejected.
     */
    inline bool IsAABBVisible(const std::array<glm::vec4, 6>& planes,
                               const glm::vec3& bMin,
                               const glm::vec3& bMax)
    {
        for (const auto& p : planes)
        {
            const glm::vec3 pv = {
                p.x >= 0.f ? bMax.x : bMin.x,
                p.y >= 0.f ? bMax.y : bMin.y,
                p.z >= 0.f ? bMax.z : bMin.z,
            };
            if (glm::dot(glm::vec3(p), pv) + p.w < 0.f)
                return false;
        }
        return true;
    }

} // namespace AE::World::Frustum
