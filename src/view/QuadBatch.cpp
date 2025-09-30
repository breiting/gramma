#include <gramma/view/QuadBatch.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

QuadBatch::~QuadBatch() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void QuadBatch::Init() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // pos x,y (2 floats), color r,g,b,a (4 floats) = 6 floats per vertex, 4 vertices per quad = 24 floats
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));  // color
    glEnableVertexAttribArray(1);
}

void QuadBatch::Clear() {
    m_vertices.clear();
    m_quadCount = 0;
}

void QuadBatch::AddQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    // Triangle 1: Bottom-left, Bottom-right, Top-right
    // Bottom-left
    m_vertices.push_back(pos.x);
    m_vertices.push_back(pos.y);
    m_vertices.push_back(color.r);
    m_vertices.push_back(color.g);
    m_vertices.push_back(color.b);
    m_vertices.push_back(color.a);

    // Bottom-right
    m_vertices.push_back(pos.x + size.x);
    m_vertices.push_back(pos.y);
    m_vertices.push_back(color.r);
    m_vertices.push_back(color.g);
    m_vertices.push_back(color.b);
    m_vertices.push_back(color.a);

    // Top-right
    m_vertices.push_back(pos.x + size.x);
    m_vertices.push_back(pos.y + size.y);
    m_vertices.push_back(color.r);
    m_vertices.push_back(color.g);
    m_vertices.push_back(color.b);
    m_vertices.push_back(color.a);

    // Triangle 2: Bottom-left, Top-right, Top-left
    // Bottom-left
    m_vertices.push_back(pos.x);
    m_vertices.push_back(pos.y);
    m_vertices.push_back(color.r);
    m_vertices.push_back(color.g);
    m_vertices.push_back(color.b);
    m_vertices.push_back(color.a);

    // Top-right
    m_vertices.push_back(pos.x + size.x);
    m_vertices.push_back(pos.y + size.y);
    m_vertices.push_back(color.r);
    m_vertices.push_back(color.g);
    m_vertices.push_back(color.b);
    m_vertices.push_back(color.a);

    // Top-left
    m_vertices.push_back(pos.x);
    m_vertices.push_back(pos.y + size.y);
    m_vertices.push_back(color.r);
    m_vertices.push_back(color.g);
    m_vertices.push_back(color.b);
    m_vertices.push_back(color.a);

    m_quadCount++;
}

void QuadBatch::Upload() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_DYNAMIC_DRAW);
}

void QuadBatch::Draw(const Shader& shader) const {
    if (m_quadCount == 0) return;
    shader.Bind();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_quadCount * 6);
}

}  // namespace gr