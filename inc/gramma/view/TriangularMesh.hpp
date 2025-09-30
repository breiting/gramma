#pragma once
#include <glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/view/IRenderable.hpp>
#include <vector>

namespace gr {

class Shader;

/** 2D triangular mesh with force-based networks. */
class TriangularMesh : public IRenderable {
   public:
    TriangularMesh() = default;
    ~TriangularMesh();

    void Init() override;
    void Clear() override;
    void AddNode(const glm::vec2& pos);
    void AddEdge(int node1, int node2);
    void ApplyForces();                                            // placeholder for physics
    void Create(const std::vector<float>& vertices, int attribs);  // finalize mesh
    void Upload() override;
    void Draw(const Shader& shader, const glm::mat4& vp, const glm::vec4& color = glm::vec4(1.0f)) override;

    /** Bind the VAO. */
    void Bind() const {
        glBindVertexArray(m_Vao);
    }

    /** @return Number of vertices. */
    size_t VertexCount() const {
        return m_VertexCount;
    }

   private:
    GLuint m_Vao = 0, m_Vbo = 0;
    size_t m_VertexCount = 0;
    std::vector<glm::vec2> m_Nodes;
    std::vector<std::pair<int, int>> m_Edges;
    std::vector<float> m_Vertices;  // for rendering
};

}  // namespace gr
