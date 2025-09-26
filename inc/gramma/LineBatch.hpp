#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace gramma {

/** Simple polyline batch in world units. */
class LineBatch {
   public:
    bool init();
    void set(const std::vector<glm::vec2>& pts);  // pairs are drawn as segments
    void draw(const glm::mat4& vp, float alpha = 1.0f) const;

   private:
    unsigned m_vao = 0, m_vbo = 0, m_prog = 0;
    int m_count = 0;
};

}  // namespace gramma
