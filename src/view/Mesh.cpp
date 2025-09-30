#include <gramma/view/Mesh.hpp>

namespace gr {

Mesh::~Mesh() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Create(const std::vector<float>& vertices, int attribs) {
    m_vertexCount = vertices.size() / attribs;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, attribs, GL_FLOAT, GL_FALSE, attribs * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

}  // namespace gr