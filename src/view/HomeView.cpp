#include <glad.h>

#include <cmath>
#include <glm/trigonometric.hpp>
#include <gramma/view/HomeView.hpp>
#include <gramma/view/Uniforms.hpp>
#include <vector>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace gr {

HomeView::HomeView() = default;

HomeView::~HomeView() {
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
}

void HomeView::Init() {
    // Hexagon calculation
    std::vector<glm::vec2> vertices;
    const int sides = 6;
    const float radius = 1.0f;
    for (int i = 0; i <= sides; ++i) {
        float angle = glm::radians(60.0f * i);
        vertices.push_back({std::cos(angle) * radius, std::sin(angle) * radius});
    }
    m_VertexCount = static_cast<int>(vertices.size());

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glBindVertexArray(0);

    m_Shader.BuildLine();
}

void HomeView::Draw(const Home* home, const glm::mat4& vp) {
    if (!home) return;

    glm::vec2 pos = home->GetPosition();

    float scale = 0.5f + 0.1f * home->GetMaxOccupancy();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

    auto mvp = vp * model;

    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, mvp);
    m_Shader.SetVec4(Uniforms::COLOR, glm::vec4(0.8f, 0.2f, 0.8f, 1.0f));  // lila
    m_Shader.SetFloat("uAlpha", 1.0);

    glBindVertexArray(m_Vao);
    glDrawArrays(GL_LINE_STRIP, 0, m_VertexCount);
    glBindVertexArray(0);
}

}  // namespace gr
