#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace AE::World { class World; }

namespace AE
{
    /**
     * @brief Sky and atmosphere configuration.
     *
     * Controls the procedural sky gradient and sun disc rendered behind the terrain.
     * All colours are linear RGB. sunDirection is normalised automatically at scene init.
     *
     * Example:
     * @code
     * .sky = { .zenithColor = {0.05f, 0.15f, 0.45f}, .sunSize = 0.008f }
     * @endcode
     */
    struct SkySettings
    {
        /** Set to false to disable sky rendering entirely (background stays clear colour). */
        bool enabled = true;

        /** Colour at the top of the sky dome. */
        glm::vec3 zenithColor  = {0.10f, 0.40f, 0.80f};

        /** Colour at the horizon. */
        glm::vec3 horizonColor = {0.60f, 0.80f, 1.00f};

        /** Colour of the lower hemisphere (below horizon). */
        glm::vec3 groundColor  = {0.25f, 0.20f, 0.15f};

        /** World-space direction toward the sun. Normalised automatically at init. */
        glm::vec3 sunDirection = {0.60f, 1.00f, 0.40f};

        /** Sun disc colour. */
        glm::vec3 sunColor     = {1.00f, 0.95f, 0.80f};

        /** Controls the angular size of the sun disc (0.005 = small, 0.02 = large). */
        float sunSize = 0.005f;

        /** Path to the sky .glsl shader file. */
        std::string skyShaderPath = "assets/shaders/sky.glsl";
    };


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

        // --- Editing ---

        /** Initial radius (in block-index units) of the dig/fill sphere.
         *  Can also be changed at runtime via VoxelScene::SetEditRadius(). */
        float editRadius = 2.0f;

        // --- Camera ---

        /** Vertical field of view in degrees. */
        float fov = 45.0f;

        /** Near clip plane distance in block-index units. Multiplied by blockScale at runtime. */
        float nearClip = 0.01f;

        /** Far clip plane distance in block-index units. Multiplied by blockScale at runtime.
         *  1000 blocks exceeds the default 256-block world diameter at any blockScale. */
        float farClip = 1000.0f;

        /** Initial aspect ratio. Updated automatically on window resize. */
        float aspectRatio = 1280.0f / 720.0f;

        // --- Sky ---

        /** Procedural sky / atmosphere settings. */
        SkySettings sky;

        // --- Shaders ---

        /** Path to the terrain .glsl shader (vertex + fragment separated by #type directives).
         *  Defaults to the built-in terrain shader. Supply a custom path to use your own. */
        std::string terrainShaderPath = "assets/shaders/textured.glsl";

        /** Path to the edit-cursor highlight .glsl shader file. */
        std::string highlightShaderPath = "assets/shaders/highlight.glsl";

        // --- World generation ---

        /** Optional custom terrain generator.  When non-null, called instead of the default
         *  FBM procedural terrain.  Signature: void(World::World& world, uint32_t seed).
         *  Leave null to use the built-in terrain. */
        std::function<void(World::World&, uint32_t)> worldGenerator = nullptr;
    };
}
