#pragma once
#include <glm/vec2.hpp>

namespace gr {

class QuadBatch;

/** Base class for HUD elements. */
class Element {
   public:
    Element(const glm::vec2& pos) : m_pos(pos) {
    }
    virtual ~Element() = default;

    /** Render the element. */
    virtual void Render(QuadBatch& qb) const = 0;

   protected:
    glm::vec2 m_pos;  // relative to panel
};

}  // namespace gr