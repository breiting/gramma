#pragma once
#include <glad.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace gr {

class Shader;

/** Simple batch for drawing filled quads (rectangles). */
class QuadBatch {
   public:
    QuadBatch() = default;
    ~QuadBatch();

    void Init();
    void Clear();
    void AddQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    void Upload();
    void Draw(const Shader& shader) const;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    std::vector<float> m_vertices;  // pos x,y, color r,g,b,a
    size_t m_quadCount = 0;
};

}  // namespace gr