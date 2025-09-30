#pragma once
#include <glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <gramma/view/IRenderable.hpp>
#include <vector>

namespace gr {

class Shader;

/** Batch for drawing filled quads (rectangles). */
class QuadShapes : public IRenderable {
   public:
    QuadShapes() = default;
    ~QuadShapes();

    void Init() override;
    void Clear() override;
    void AddQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    void Upload() override;
    void Draw(const Shader& shader, const glm::mat4& vp, const glm::vec4& color = glm::vec4(1.0f)) override;

   private:
    GLuint m_Vao = 0, m_Vbo = 0;
    std::vector<float> m_Vertices;  // pos x,y, color r,g,b,a
    size_t m_QuadCount = 0;
};

}  // namespace gr
