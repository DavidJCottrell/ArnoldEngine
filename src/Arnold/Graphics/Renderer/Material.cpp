#include "aepch.h"
#include "Material.h"

namespace AE::Graphics::Renderer
{
    Material::Material(std::shared_ptr<Shader> shader)
        : m_Shader(std::move(shader))
    {
    }

    std::shared_ptr<Material> Material::Create(const std::shared_ptr<Shader>& shader)
    {
        return std::shared_ptr<Material>(new Material(shader));
    }

    void Material::Bind() const
    {
        m_Shader->Bind();

        for (const auto& [slot, texture] : m_Textures)
            texture->Bind(slot);

        for (const auto& [name, value] : m_IntUniforms)
            m_Shader->UploadUniformInt(name, value);

        for (const auto& [name, value] : m_FloatUniforms)
            m_Shader->UploadUniformFloat(name, value);

        for (const auto& [name, value] : m_Float3Uniforms)
            m_Shader->UploadUniformFloat3(name, value);

        for (const auto& [name, value] : m_Float4Uniforms)
            m_Shader->UploadUniformFloat4(name, value);

        for (const auto& [name, value] : m_Mat4Uniforms)
            m_Shader->UploadUniformMat4(name, value);
    }

    void Material::SetTexture(uint32_t slot, const std::shared_ptr<Texture2D>& texture)
    {
        m_Textures[slot] = texture;
    }

    void Material::SetInt(const std::string& name, int value)
    {
        m_IntUniforms[name] = value;
    }

    void Material::SetFloat(const std::string& name, float value)
    {
        m_FloatUniforms[name] = value;
    }

    void Material::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        m_Float3Uniforms[name] = value;
    }

    void Material::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        m_Float4Uniforms[name] = value;
    }

    void Material::SetMat4(const std::string& name, const glm::mat4& value)
    {
        m_Mat4Uniforms[name] = value;
    }
}
