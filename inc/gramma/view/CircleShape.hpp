#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/view/IRenderable.hpp>
#include <vector>

namespace gr {

class Shader;

/** Simple filled/non-filled circle in world units (meters). */
class CircleShape : public IRenderable {
   public:
    ~CircleShape();
    void Init() override;
    void Clear() override;  // reset
    void Set(const glm::vec2& pos, float diameter, int segments = 64);
    void SetFilled(bool filled);
    void Draw(const Shader& shader, const glm::mat4& vp, const glm::vec4& color = glm::vec4(1.0f)) override;

   private:
    unsigned m_Vao = 0, m_Vbo = 0;
    bool m_Filled{false};
    std::vector<glm::vec2> m_Points;
};

}  // namespace gr
