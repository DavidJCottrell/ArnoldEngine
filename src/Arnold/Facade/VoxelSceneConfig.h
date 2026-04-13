#pragma once

#include <cstdint>

namespace AE
{
    /**
     * @brief Configuration for a VoxelScene.
     *
     * Pass an instance to VoxelLayer or VoxelScene to control terrain seed,
     * block size, and camera parameters. All fields have sensible defaults
     * so a minimal game only needs to set the fields it cares about.
     *
     * Example:
     * @code
     * MyGame() : VoxelLayer({.blockScale = 0.1f, .seed = 42}) {}
     * @endcode
     */
    struct VoxelSceneConfig
    {
        // --- World ---

        /** Uniform scale applied to every block at render time.
         *  1.0 = Minecraft-style units. 0.1 = pixel-like, 10 blocks per unit. */
        float blockScale = 1.0f;

        /** Terrain generation seed passed to WorldGenerator::GenerateTerrain. */
        uint32_t seed = 0;

        // --- Camera ---

        /** Vertical field of view in degrees. */
        float fov = 45.0f;

        /** Near clip plane (multiplied by blockScale automatically). */
        float nearClip = 0.01f;

        /** Far clip plane in world units. Not scaled — stays large enough for any blockScale. */
        float farClip = 1000.0f;

        /** Initial aspect ratio. Updated automatically on window resize. */
        float aspectRatio = 1280.0f / 720.0f;
    };
}
