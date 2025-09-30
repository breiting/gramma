#include <gramma/view/Hud.hpp>
#include <gramma/view/Panel.hpp>

namespace gr {

Panel::Panel(const glm::vec2& pos, const glm::vec2& size, QuadShapes& qb, const glm::vec4& bgColor,
             const glm::vec4& borderColor, float borderWidth)
    : m_Pos(pos),
      m_Size(size),
      m_QuadShapes(qb),
      m_BgColor(bgColor),
      m_BorderColor(borderColor),
      m_BorderWidth(borderWidth) {
}

void Panel::AddElement(std::unique_ptr<Element> element) {
    m_Elements.push_back(std::move(element));
}

void Panel::Render() const {
    // Render background
    m_QuadShapes.AddQuad(m_Pos, m_Size, m_BgColor);

    // Render border (as thinner quads around edges)
    float bw = m_BorderWidth;
    // Top
    m_QuadShapes.AddQuad(m_Pos + glm::vec2(0, m_Size.y - bw), glm::vec2(m_Size.x, bw), m_BorderColor);
    // Bottom
    m_QuadShapes.AddQuad(m_Pos, glm::vec2(m_Size.x, bw), m_BorderColor);
    // Left
    m_QuadShapes.AddQuad(m_Pos + glm::vec2(0, bw), glm::vec2(bw, m_Size.y - 2 * bw), m_BorderColor);
    // Right
    m_QuadShapes.AddQuad(m_Pos + glm::vec2(m_Size.x - bw, bw), glm::vec2(bw, m_Size.y - 2 * bw), m_BorderColor);

    // Render elements
    for (const auto& elem : m_Elements) {
        elem->Render(m_QuadShapes);
    }
}

}  // namespace gr