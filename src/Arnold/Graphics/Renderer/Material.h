#pragma once

#include "Arnold/Graphics/Renderer/Shader.h"
#include "Arnold/Graphics/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace AE::Graphics::Renderer
{
    /**
     * @class Material
     * @brief Bundles a Shader, Texture bindings, and named uniform values into one object.
     *
     * Calling Bind() on a Material replaces the manual sequence of:
     *   shader->Bind(); texture->Bind(slot); shader->UploadUniform*(...);
     *
     * Per-draw-call uniforms (u_ViewProjection, u_Transform) are NOT stored here —
     * those are Renderer concerns uploaded inside Renderer::Submit.
     *
     * Usage:
     * @code
     * auto mat = Material::Create(Shader::Create("assets/shaders/textured.glsl"));
     * mat->SetTexture(0, Texture2D::Create("assets/textures/grass.png"));
     * mat->SetInt("u_Texture", 0);
     * mat->SetFloat3("u_LightDir", glm::normalize(glm::vec3(1, 2, 0.5f)));
     *
     * Renderer::Submit(mat, mesh);
     * @endcode
     */
    class Material
    {
    public:
        static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader);

        /** Binds the shader, all textures, and uploads all stored uniforms. */
        void Bind() const;

        void SetTexture(uint32_t slot, const std::shared_ptr<Texture2D>& texture);
        void SetInt    (const std::string& name, int value);
        void SetFloat  (const std::string& name, float value);
        void SetFloat3 (const std::string& name, const glm::vec3& value);
        void SetFloat4 (const std::string& name, const glm::vec4& value);
        void SetMat4   (const std::string& name, const glm::mat4& value);

        [[nodiscard]] const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }

    private:
        explicit Material(std::shared_ptr<Shader> shader);

        std::shared_ptr<Shader> m_Shader;

        std::unordered_map<uint32_t, std::shared_ptr<Texture2D>> m_Textures;
        std::unordered_map<std::string, int>       m_IntUniforms;
        std::unordered_map<std::string, float>     m_FloatUniforms;
        std::unordered_map<std::string, glm::vec3> m_Float3Uniforms;
        std::unordered_map<std::string, glm::vec4> m_Float4Uniforms;
        std::unordered_map<std::string, glm::mat4> m_Mat4Uniforms;
    };
}
