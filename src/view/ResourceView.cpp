#include <glad.h>

#include <gramma/view/ResourceView.hpp>

namespace gr {

void ResourceView::Init() {
    m_Shader.BuildResource();
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

void ResourceView::Draw(const IResource* res, const glm::mat4& vp) {
    m_Shader.Bind();
    m_Shader.SetMat4("uMVP", vp);
    m_Shader.SetVec2("uPos", res->GetPosition());

    // Nutrition beeinflusst Größe
    float baseRadius = 0.6f;  // "volle" ResourceSource
    // float radius = baseRadius * (0.2f + 0.8f * res->GetType());
    float radius = baseRadius;  // TODO: dependent on resource type
    m_Shader.SetFloat("uRadius", radius);

    m_Shader.SetFloat("uBlendWidth", 0.05f);               // dünner soften Rand
    m_Shader.SetVec4("uColor", {0.2f, 0.8f, 0.2f, 1.0f});  // grün

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

}  // namespace gr
