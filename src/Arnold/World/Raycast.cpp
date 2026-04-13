#include "Raycast.h"
#include "World.h"

#include <glm/glm.hpp>
#include <cmath>

namespace AE::World
{
    RaycastResult Raycast(const World& world,
                          const glm::vec3& origin,
                          const glm::vec3& direction,
                          float maxDistance)
    {
        const glm::vec3 dir = glm::normalize(direction);
        constexpr float STEP         = 0.1f;
        constexpr int   BISECT_ITERS = 8;

        float prevD = world.SampleDensity(origin.x, origin.y, origin.z);

        for (float t = STEP; t <= maxDistance; t += STEP)
        {
            const glm::vec3 pos = origin + dir * t;
            const float d = world.SampleDensity(pos.x, pos.y, pos.z);

            // Crossed from air (<0) into solid (≥0) — ray entered the terrain surface
            if (prevD < 0.0f && d >= 0.0f)
            {
                // Binary-search for the precise crossing
                // tLo is in air (density < 0), tHi is in solid (density >= 0)
                float tLo = t - STEP, tHi = t;
                for (int i = 0; i < BISECT_ITERS; ++i)
                {
                    const float tMid = 0.5f * (tLo + tHi);
                    const glm::vec3 mp = origin + dir * tMid;
                    if (world.SampleDensity(mp.x, mp.y, mp.z) < 0.0f) tLo = tMid;  // still air
                    else                                                 tHi = tMid;  // solid
                }

                const glm::vec3 hit = origin + dir * (0.5f * (tLo + tHi));

                // Outward normal from density gradient (central difference).
                // Step matches ChunkMeshBuilder::ComputeNormal for consistent fill normals.
                constexpr float h = 0.5f;
                const float gx = world.SampleDensity(hit.x+h, hit.y,   hit.z  )
                               - world.SampleDensity(hit.x-h, hit.y,   hit.z  );
                const float gy = world.SampleDensity(hit.x,   hit.y+h, hit.z  )
                               - world.SampleDensity(hit.x,   hit.y-h, hit.z  );
                const float gz = world.SampleDensity(hit.x,   hit.y,   hit.z+h)
                               - world.SampleDensity(hit.x,   hit.y,   hit.z-h);

                const glm::vec3 grad(gx, gy, gz);
                const float len = glm::length(grad);
                const glm::vec3 normal = (len > 1e-6f) ? (-grad / len) : glm::vec3(0.f, 1.f, 0.f);

                return {true, hit, normal};
            }

            prevD = d;
        }

        return {};
    }
}
