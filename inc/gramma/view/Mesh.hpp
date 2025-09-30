#pragma once
#include <glad.h>

#include <vector>

namespace gr {

/** Simple mesh wrapper for VAO/VBO. */
class Mesh {
   public:
    Mesh() = default;
    ~Mesh();

    /** Create VAO and VBO, upload vertex data.
     * @param vertices Vector of floats (e.g., positions).
     * @param attribs Number of attributes per vertex.
     */
    void Create(const std::vector<float>& vertices, int attribs);

    /** Bind the VAO. */
    void Bind() const {
        glBindVertexArray(m_vao);
    }

    /** @return Number of vertices. */
    size_t VertexCount() const {
        return m_vertexCount;
    }

   private:
    GLuint m_vao = 0, m_vbo = 0;
    size_t m_vertexCount = 0;
};

}  // namespace gr