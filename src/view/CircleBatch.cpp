#include <glad.h>

#include <gramma/view/CircleBatch.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

bool CircleBatch::init() {
    // quad geometry
    float quad[8] = {-1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f};
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboQuad);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_vboInst);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboInst);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // instances: pos (loc=1), size (loc=2)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)offsetof(Instance, diameter));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    return true;
}

void CircleBatch::clear() {
    m_data.clear();
}
void CircleBatch::add(const glm::vec2& pos, float diameter) {
    m_data.push_back({pos, diameter});
}
void CircleBatch::upload() {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboInst);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(Instance), m_data.data(), GL_DYNAMIC_DRAW);
}

void CircleBatch::draw(const Shader& shader, const glm::mat4& vp, float alpha) const {
    shader.Bind();
    shader.SetMat4("uVP", vp);
    shader.SetFloat("uAlpha", alpha);
    glBindVertexArray(m_vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)m_data.size());
}

}  // namespace gr
