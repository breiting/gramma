#include <gramma/view/Hud.hpp>
#include <gramma/view/Panel.hpp>
#include <gramma/view/TextElement.hpp>

namespace gr {

TextElement::TextElement(const glm::vec2& pos, const std::string& text, const glm::vec4& color, Font* font)
    : Element(pos), m_text(text), m_color(color), m_font(font) {
}

void TextElement::Render(QuadBatch& qb) const {
    if (!m_font) return;

    glm::vec2 currentPos = m_pos;  // assume pos is relative to panel, but for simplicity, treat as screen pos
    for (char c : m_text) {
        if (c == '\n') {
            currentPos.x = m_pos.x;
            currentPos.y += 20.0f;
            continue;
        }

        GLuint tex = m_font->GetCharTexture(c);
        if (!tex) continue;

        stbtt_bakedchar* data = m_font->GetCharData(c);

        // Render quad for char
        glm::vec2 size(data->x1 - data->x0, data->y1 - data->y0);
        glm::vec2 offset(data->xoff, data->yoff);
        qb.AddQuad(currentPos + offset, size, m_color);

        currentPos.x += data->xadvance;
    }
}

}  // namespace gr