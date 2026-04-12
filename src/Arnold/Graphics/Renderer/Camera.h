#pragma once

#include "glm/glm.hpp"

namespace AE::Graphics::Renderer
{
    /**
     * @class Camera
     * @brief Abstract base class for all camera types
     *
     * Provides a common interface for cameras so that the renderer
     * can accept any camera type (orthographic, perspective, etc.)
     * through a single BeginScene overload.
     */
    class Camera
    {
    public:
        virtual ~Camera() = default;
        [[nodiscard]] virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
    };
}
