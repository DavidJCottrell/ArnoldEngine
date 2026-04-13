#pragma once

#include <array>

namespace AE::World
{
    class Chunk
    {
    public:
        static constexpr int SIZE         = 16;
        static constexpr int DENSITY_SIZE = SIZE + 1;  // 17 — one extra per axis for boundary overlap

        Chunk() { m_Density.fill(0.0f); }

        /** Read stored density at local grid position. x/y/z ∈ [0, DENSITY_SIZE). */
        [[nodiscard]] float GetDensity(int x, int y, int z) const { return m_Density[Index(x, y, z)]; }

        /** Write stored density at local grid position. x/y/z ∈ [0, DENSITY_SIZE). */
        void SetDensity(int x, int y, int z, float v) { m_Density[Index(x, y, z)] = v; }

        [[nodiscard]] bool IsInBounds(int x, int y, int z) const
        {
            return x >= 0 && x < DENSITY_SIZE
                && y >= 0 && y < DENSITY_SIZE
                && z >= 0 && z < DENSITY_SIZE;
        }

    private:
        static int Index(int x, int y, int z) { return x + DENSITY_SIZE * (y + DENSITY_SIZE * z); }

        // 17³ = 4913 floats (~19.7 KB per chunk).
        std::array<float, DENSITY_SIZE * DENSITY_SIZE * DENSITY_SIZE> m_Density;
    };
}
