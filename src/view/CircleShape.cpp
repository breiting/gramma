#include <glad.h>

#include <gramma/util/Math.hpp>
#include <gramma/view/CircleShape.hpp>
#include <gramma/view/Shader.hpp>
#include <gramma/view/Uniforms.hpp>
#include <vector>

namespace gr {

CircleShape::~CircleShape() {
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
}

void CircleShape::Clear() {
    m_Points.clear();
}

void CircleShape::Init() {
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CircleShape::SetFilled(bool filled) {
    m_Filled = filled;
}

void CircleShape::Set(const glm::vec2& center, float radius, int segments) {
    m_Points.clear();
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        glm::vec2 point = center + radius * glm::vec2(cos(angle), sin(angle));
        m_Points.push_back(point);
    }
    // Close the circle
    m_Points.push_back(m_Points[m_Points.size() - segments]);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, m_Points.size() * 2 * sizeof(float), m_Points.data(), GL_STATIC_DRAW);
}

void CircleShape::Draw(const Shader& shader, const glm::mat4& vp, const glm::vec4& color) {
    if (m_Points.empty()) return;

    shader.Bind();
    shader.SetMat4(Uniforms::MVP, vp);
    shader.SetVec4(Uniforms::COLOR, color);
    glBindVertexArray(m_Vao);

    if (m_Filled) {
        glDrawArrays(GL_TRIANGLE_FAN, 0, m_Points.size());
    } else {
        glDrawArrays(GL_LINE_LOOP, 0, m_Points.size());
    }
}

}  // namespace gr
