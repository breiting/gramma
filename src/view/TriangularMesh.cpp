#include <gramma/view/Shader.hpp>
#include <gramma/view/TriangularMesh.hpp>

namespace gr {

TriangularMesh::~TriangularMesh() {
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
}

void TriangularMesh::Init() {
    // Init if needed
}

void TriangularMesh::Clear() {
    m_Nodes.clear();
    m_Edges.clear();
    m_Vertices.clear();
    m_VertexCount = 0;
}

void TriangularMesh::AddNode(const glm::vec2& pos) {
    m_Nodes.push_back(pos);
}

void TriangularMesh::AddEdge(int node1, int node2) {
    m_Edges.emplace_back(node1, node2);
}

void TriangularMesh::ApplyForces() {
    // Placeholder for physics simulation
    // Update node positions based on forces
}

void TriangularMesh::Create(const std::vector<float>& vertices, int attribs) {
    m_Vertices = vertices;
    m_VertexCount = vertices.size() / attribs;
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
    if (attribs >= 2) {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attribs * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    if (attribs >= 6) {
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, attribs * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
}

void TriangularMesh::Upload() {
    // Update buffer if needed
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(float), m_Vertices.data());
}

void TriangularMesh::Draw(const Shader& shader, const glm::mat4& vp, float alpha) {
    if (m_VertexCount == 0) return;
    shader.Bind();
    shader.SetMat4("uVP", vp);
    shader.SetFloat("uAlpha", alpha);
    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);  // Assume triangulated
}

}  // namespace gr