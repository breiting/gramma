#include <gramma/view/Hud.hpp>
#include <gramma/view/Panel.hpp>
#include <gramma/view/TextElement.hpp>

#include "gramma/view/TexturedQuadShapes.hpp"

namespace gr {

TextElement::TextElement(const glm::vec2& pos, const std::string& text, const glm::vec4& color, Font* font)
    : Element(pos), m_text(text), m_color(color), m_font(font) {
}

void TextElement::Render(TexturedQuadShapes& qb) const {
    if (!m_font) return;

    glm::vec2 currentPos = m_Pos;  // assume pos is relative to panel, but for simplicity, treat as screen pos
    for (char c : m_text) {
        if (c == '\n') {
            currentPos.x = m_Pos.x;
            currentPos.y += 20.0f;
            continue;
        }

        GLuint tex = m_font->GetCharTexture(c);
        if (!tex) continue;

        stbtt_bakedchar* data = m_font->GetCharData(c);

        // Render quad for char
        glm::vec2 size(data->x1 - data->x0, data->y1 - data->y0);
        glm::vec2 offset(data->xoff, data->yoff);
        // uvRect: u0 = x0/512, v0 = y0/512, u1 = x1/512, v1 = y1/512
        // glm::vec4 uvRect(data->x0 / 512.0f, data->y0 / 512.0f, data->x1 / 512.0f, data->y1 / 512.0f);
        glm::vec4 uvRect(data->x0 / 512.0f, data->y1 / 512.0f, data->x1 / 512.0f, data->y0 / 512.0f);
        qb.AddQuad(currentPos + offset, size, uvRect, m_color, tex);

        currentPos.x += data->xadvance;
    }
}

}  // namespace gr
