#include <glad.h>

#include <gramma/view/CircleShapes.hpp>
#include <gramma/view/Shader.hpp>
#include <gramma/view/Uniforms.hpp>

namespace gr {

void CircleShapes::Init() {
    // quad geometry
    float quad[8] = {-1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f};
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);

    glGenBuffers(1, &m_VboQuad);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_VboInst);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboInst);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // instances: pos (loc=1), size (loc=2), color (loc=3)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)offsetof(Instance, Diameter));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)offsetof(Instance, Color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
}

void CircleShapes::Clear() {
    m_Data.clear();
}
void CircleShapes::Add(const glm::vec2& pos, float diameter, const glm::vec4& color) {
    m_Data.push_back({pos, diameter, color});
}
void CircleShapes::Upload() {
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboInst);
    glBufferData(GL_ARRAY_BUFFER, m_Data.size() * sizeof(Instance), m_Data.data(), GL_DYNAMIC_DRAW);
}

void CircleShapes::Draw(const Shader& shader, const glm::mat4& vp, const glm::vec4& color) {
    shader.Bind();
    shader.SetMat4(Uniforms::MVP, vp);
    shader.SetFloat("uAlpha", color.a);
    glBindVertexArray(m_Vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)m_Data.size());
}

}  // namespace gr
