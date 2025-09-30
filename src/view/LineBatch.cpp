#include <glad.h>

#include <gramma/view/LineBatch.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

bool LineBatch::init() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return true;
}

void LineBatch::set(const std::vector<glm::vec2>& pts) {
    m_count = (int)pts.size();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_count * 2 * sizeof(float), pts.data(), GL_DYNAMIC_DRAW);
}

void LineBatch::draw(const Shader& shader, const glm::mat4& vp, float alpha) const {
    if (m_count <= 0) return;
    shader.Bind();
    shader.SetMat4("uVP", vp);
    shader.SetFloat("uAlpha", alpha);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, m_count);
}

}  // namespace gr
