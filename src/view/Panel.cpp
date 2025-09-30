#include <gramma/view/Hud.hpp>
#include <gramma/view/Panel.hpp>
#include <gramma/view/TexturedQuadShapes.hpp>

namespace gr {

Panel::Panel(const glm::vec2& pos, const glm::vec2& size, TexturedQuadShapes& qb, const glm::vec4& bgColor,
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
    m_QuadShapes.AddQuad(m_Pos, m_Size, glm::vec4(0, 0, 0, 0), m_BgColor, 0);

    // Render border (as thinner quads around edges)
    float bw = m_BorderWidth;
    // Top
    m_QuadShapes.AddQuad(m_Pos + glm::vec2(0, m_Size.y - bw), glm::vec2(m_Size.x, bw), glm::vec4(0, 0, 0, 0),
                         m_BorderColor, 0);
    // Bottom
    m_QuadShapes.AddQuad(m_Pos, glm::vec2(m_Size.x, bw), glm::vec4(0, 0, 0, 0), m_BorderColor, 0);
    // Left
    m_QuadShapes.AddQuad(m_Pos + glm::vec2(0, bw), glm::vec2(bw, m_Size.y - 2 * bw), glm::vec4(0, 0, 0, 0),
                         m_BorderColor, 0);
    // Right
    m_QuadShapes.AddQuad(m_Pos + glm::vec2(m_Size.x - bw, bw), glm::vec2(bw, m_Size.y - 2 * bw), glm::vec4(0, 0, 0, 0),
                         m_BorderColor, 0);

    // Render elements
    for (const auto& elem : m_Elements) {
        elem->Render(m_QuadShapes);
    }
}

}  // namespace gr