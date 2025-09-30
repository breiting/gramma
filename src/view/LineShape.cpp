#include <glad.h>

#include <gramma/view/LineShape.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

void LineShape::Init() {
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void LineShape::Clear() {
    m_Points.clear();
    m_Count = 0;
}

void LineShape::Set(const std::vector<glm::vec2>& pts) {
    m_Points = pts;
    m_Count = (int)pts.size();
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, m_Count * 2 * sizeof(float), pts.data(), GL_DYNAMIC_DRAW);
}

void LineShape::AddCircleOutline(const glm::vec2& center, float radius, int segments) {
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * 3.14159265359f * i / segments;
        glm::vec2 point = center + radius * glm::vec2(cos(angle), sin(angle));
        m_Points.push_back(point);
    }
    // Close the circle
    m_Points.push_back(m_Points[m_Points.size() - segments]);
    m_Count = (int)m_Points.size();
}

void LineShape::Upload() {
    // Already uploaded in Set
}

void LineShape::Draw(const Shader& shader, const glm::mat4& vp, float alpha) {
    if (m_Count <= 0) return;
    shader.Bind();
    shader.SetMat4("uVP", vp);
    shader.SetFloat("uAlpha", alpha);
    glBindVertexArray(m_Vao);
    glDrawArrays(GL_LINES, 0, m_Count);
}

}  // namespace gr
