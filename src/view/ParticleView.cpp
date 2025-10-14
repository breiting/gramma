// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <gramma/view/ParticleView.hpp>
#include <gramma/view/Uniforms.hpp>

namespace gr {

static glm::vec4 GetColor(int group) {
    switch (group) {
        case 0:
            return {1.0f, 1.0f, 0.0f, 1.0f};  // yellow
        case 1:
            return {0.0f, 1.0f, 0.0f, 1.0f};  // green
        case 2:
            return {0.0f, 0.5f, 1.0f, 1.0f};  // blue
        case 3:
            return {1.0f, 0.0f, 0.0f, 1.0f};  // red
    }
    return {1.0f, 1.0f, 1.0f, 1.0f};  // fallback
}

ParticleView::ParticleView() {
}

ParticleView::~ParticleView() {
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
}

void ParticleView::Init() {
    m_Shader.BuildParticle();

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);

    // Layout (binding für Shader-Instanced Attributes)
    std::size_t stride = sizeof(InstanceData);

    // ----- Body VAO -----
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

    glEnableVertexAttribArray(1);  // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);  // radius
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, radius));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);  // color
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void ParticleView::UpdateInstances(const std::vector<std::unique_ptr<Particle>>& particles) {
    m_InstanceData.clear();
    m_InstanceData.reserve(particles.size());

    for (auto& particle : particles) {
        auto color = GetColor(particle->GetGroup());

        InstanceData d;
        d.position = particle->GetPosition();
        d.radius = particle->GetRadius();
        d.color = color;
        m_InstanceData.push_back(d);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, m_InstanceData.size() * sizeof(InstanceData), m_InstanceData.data(), GL_DYNAMIC_DRAW);
}

void ParticleView::Draw(const glm::mat4& vp) {
    if (m_InstanceData.empty()) return;

    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, vp);
    glBindVertexArray(m_Vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(m_InstanceData.size()));

    glBindVertexArray(0);
}

}  // namespace gr
