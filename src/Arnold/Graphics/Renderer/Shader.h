#pragma once

#include <string>

namespace AE::Graphics::Renderer
{
  /**
   * @class Shader
   * @brief Manages compilation and usage of GPU shader programs
   *
   * Handles the creation, compilation, and usage of shader programs for rendering.
   * Supports vertex and fragment shaders with potential for expansion to other types.
   *
   * Features:
   * - Shader compilation and linking
   * - Error checking and reporting
   * - Program binding and unbinding
   * - Resource cleanup
   *
   * @note Currently supports GLSL shaders through OpenGL
   */
  class Shader
  {
  public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

  private:
    uint32_t m_RendererID{};
  };
}
