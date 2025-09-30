#include <gramma/view/Hud.hpp>
#include <gramma/view/Panel.hpp>

namespace gr {

Panel::Panel(const glm::vec2& pos, const glm::vec2& size, QuadBatch& qb, const glm::vec4& bgColor,
             const glm::vec4& borderColor, float borderWidth)
    : m_pos(pos),
      m_size(size),
      m_quadBatch(qb),
      m_bgColor(bgColor),
      m_borderColor(borderColor),
      m_borderWidth(borderWidth) {
}

void Panel::AddElement(std::unique_ptr<Element> element) {
    m_elements.push_back(std::move(element));
}

void Panel::Render() const {
    // Render background
    m_quadBatch.AddQuad(m_pos, m_size, m_bgColor);

    // Render border (as thinner quads around edges)
    float bw = m_borderWidth;
    // Top
    m_quadBatch.AddQuad(m_pos + glm::vec2(0, m_size.y - bw), glm::vec2(m_size.x, bw), m_borderColor);
    // Bottom
    m_quadBatch.AddQuad(m_pos, glm::vec2(m_size.x, bw), m_borderColor);
    // Left
    m_quadBatch.AddQuad(m_pos + glm::vec2(0, bw), glm::vec2(bw, m_size.y - 2 * bw), m_borderColor);
    // Right
    m_quadBatch.AddQuad(m_pos + glm::vec2(m_size.x - bw, bw), glm::vec2(bw, m_size.y - 2 * bw), m_borderColor);

    // Render elements
    for (const auto& elem : m_elements) {
        elem->Render(m_quadBatch);
    }
}

}  // namespace gr