#include <gramma/view/QuadShapes.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

QuadShapes::~QuadShapes() {
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
}

void QuadShapes::Init() {
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    // pos x,y (2 floats), color r,g,b,a (4 floats) = 6 floats per vertex, 6 vertices per quad = 36 floats
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));  // color
    glEnableVertexAttribArray(1);
}

void QuadShapes::Clear() {
    m_Vertices.clear();
    m_QuadCount = 0;
}

void QuadShapes::AddQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    // Triangle 1: Bottom-left, Bottom-right, Top-right
    // Bottom-left
    m_Vertices.push_back(pos.x);
    m_Vertices.push_back(pos.y);
    m_Vertices.push_back(color.r);
    m_Vertices.push_back(color.g);
    m_Vertices.push_back(color.b);
    m_Vertices.push_back(color.a);

    // Bottom-right
    m_Vertices.push_back(pos.x + size.x);
    m_Vertices.push_back(pos.y);
    m_Vertices.push_back(color.r);
    m_Vertices.push_back(color.g);
    m_Vertices.push_back(color.b);
    m_Vertices.push_back(color.a);

    // Top-right
    m_Vertices.push_back(pos.x + size.x);
    m_Vertices.push_back(pos.y + size.y);
    m_Vertices.push_back(color.r);
    m_Vertices.push_back(color.g);
    m_Vertices.push_back(color.b);
    m_Vertices.push_back(color.a);

    // Triangle 2: Bottom-left, Top-right, Top-left
    // Bottom-left
    m_Vertices.push_back(pos.x);
    m_Vertices.push_back(pos.y);
    m_Vertices.push_back(color.r);
    m_Vertices.push_back(color.g);
    m_Vertices.push_back(color.b);
    m_Vertices.push_back(color.a);

    // Top-right
    m_Vertices.push_back(pos.x + size.x);
    m_Vertices.push_back(pos.y + size.y);
    m_Vertices.push_back(color.r);
    m_Vertices.push_back(color.g);
    m_Vertices.push_back(color.b);
    m_Vertices.push_back(color.a);

    // Top-left
    m_Vertices.push_back(pos.x);
    m_Vertices.push_back(pos.y + size.y);
    m_Vertices.push_back(color.r);
    m_Vertices.push_back(color.g);
    m_Vertices.push_back(color.b);
    m_Vertices.push_back(color.a);

    m_QuadCount++;
}

void QuadShapes::Upload() {
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_DYNAMIC_DRAW);
}

void QuadShapes::Draw(const Shader& shader, const glm::mat4& vp, float alpha) {
    if (m_QuadCount == 0) return;
    shader.Bind();
    shader.SetMat4("uMVP", vp);
    shader.SetFloat("uAlpha", alpha);
    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLES, 0, m_QuadCount * 6);
}

}  // namespace gr