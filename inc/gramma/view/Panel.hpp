#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>

#include "Element.hpp"

namespace gr {

class QuadShapes;

/** HUD panel: rectangular container with border and background. */
class Panel {
   public:
    Panel(const glm::vec2& pos, const glm::vec2& size, QuadShapes& qb,
          const glm::vec4& bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 0.8f),
          const glm::vec4& borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float borderWidth = 2.0f);

    /** Add an element to the panel. */
    void AddElement(std::unique_ptr<Element> element);

    /** Render the panel and its elements. */
    void Render() const;

    /** Get panel position. */
    const glm::vec2& GetPos() const {
        return m_Pos;
    }

   private:
    glm::vec2 m_Pos, m_Size;
    QuadShapes& m_QuadShapes;
    glm::vec4 m_BgColor, m_BorderColor;
    float m_BorderWidth;
    std::vector<std::unique_ptr<Element>> m_Elements;
};

}  // namespace gr