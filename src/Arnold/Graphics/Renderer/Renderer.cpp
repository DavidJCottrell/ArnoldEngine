#include "aepch.h"
#include "Renderer.h"

namespace AE::Graphics::Renderer
{
    Renderer::SceneData* Renderer::m_SceneData = new SceneData;

    void Renderer::BeginScene(const Camera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
                          const std::shared_ptr<Mesh>& mesh,
                          const glm::mat4& transform
    )
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->UploadUniformMat4("u_Transform", transform);

        const auto& vertexArray = mesh->GetVertexArray();
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
