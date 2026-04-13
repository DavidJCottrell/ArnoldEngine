#pragma once

#include "glm/glm.hpp"
#include <memory>
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

    void UploadUniformInt   (const std::string& name, int value) const;
    void UploadUniformFloat (const std::string& name, float value) const;
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
    void UploadUniformMat4  (const std::string& name, const glm::mat4& matrix) const;

    /**
     * @brief Load a shader from a single .glsl file containing both stages.
     *
     * The file must contain a `#type vertex` line before the vertex source
     * and a `#type fragment` line before the fragment source. Order does not matter.
     *
     * Example file layout:
     * @code
     * #type vertex
     * #version 330 core
     * ...
     *
     * #type fragment
     * #version 330 core
     * ...
     * @endcode
     */
    static std::shared_ptr<Shader> Create(const std::string& filepath);

  private:
    uint32_t m_RendererID{};
  };
}
