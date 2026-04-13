#pragma once

#include <array>
#include <cstdint>

namespace AE::World
{
    enum class BlockType : uint8_t
    {
        Air   = 0,
        Grass = 1,
        Dirt  = 2,
        Stone = 3,
    };

    class Chunk
    {
    public:
        static constexpr int SIZE = 16;

        Chunk() { m_Blocks.fill(BlockType::Air); }

        [[nodiscard]] BlockType GetBlock(int x, int y, int z) const { return m_Blocks[Index(x, y, z)]; }
        void SetBlock(int x, int y, int z, BlockType type)          { m_Blocks[Index(x, y, z)] = type; }

        [[nodiscard]] bool IsInBounds(int x, int y, int z) const
        {
            return x >= 0 && x < SIZE && y >= 0 && y < SIZE && z >= 0 && z < SIZE;
        }

    private:
        static int Index(int x, int y, int z) { return x + SIZE * (y + SIZE * z); }

        std::array<BlockType, SIZE * SIZE * SIZE> m_Blocks;
    };
}
