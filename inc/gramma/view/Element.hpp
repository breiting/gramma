#pragma once
#include <glm/vec2.hpp>

namespace gr {

class TexturedQuadShapes;

/** Base class for HUD elements. */
class Element {
   public:
    Element(const glm::vec2& pos) : m_Pos(pos) {
    }
    virtual ~Element() = default;

    /** Render the element. */
    virtual void Render(TexturedQuadShapes& qb) const = 0;

   protected:
    glm::vec2 m_Pos;  // relative to panel
};

}  // namespace gr