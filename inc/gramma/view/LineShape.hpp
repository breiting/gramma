#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/view/IRenderable.hpp>
#include <vector>

namespace gr {

class Shader;

/** Simple polyline in world units. */
class LineShape : public IRenderable {
   public:
    void Init() override;
    void Clear() override;                        // reset
    void Set(const std::vector<glm::vec2>& pts);  // set points, pairs as segments
    void AddCircleOutline(const glm::vec2& center, float radius, int segments = 32);
    void Upload() override;  // upload to GPU
    void Draw(const Shader& shader, const glm::mat4& vp, float alpha = 1.0f) override;

   private:
    unsigned m_Vao = 0, m_Vbo = 0;
    int m_Count = 0;
    std::vector<glm::vec2> m_Points;
};

}  // namespace gr
