#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>

#include "Element.hpp"

namespace gr {

class QuadBatch;

/** HUD panel: rectangular container with border and background. */
class Panel {
   public:
    Panel(const glm::vec2& pos, const glm::vec2& size, QuadBatch& qb,
          const glm::vec4& bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 0.8f),
          const glm::vec4& borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float borderWidth = 2.0f);

    /** Add an element to the panel. */
    void AddElement(std::unique_ptr<Element> element);

    /** Render the panel and its elements. */
    void Render() const;

    /** Get panel position. */
    const glm::vec2& GetPos() const {
        return m_pos;
    }

   private:
    glm::vec2 m_pos, m_size;
    QuadBatch& m_quadBatch;
    glm::vec4 m_bgColor, m_borderColor;
    float m_borderWidth;
    std::vector<std::unique_ptr<Element>> m_elements;
};

}  // namespace gr