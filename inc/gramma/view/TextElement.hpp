#pragma once
#include <glm/vec4.hpp>
#include <string>

#include "Element.hpp"
#include "Font.hpp"
#include "gramma/view/QuadShapes.hpp"

namespace gr {

/** Text element for displaying strings. */
class TextElement : public Element {
   public:
    TextElement(const glm::vec2& pos, const std::string& text, const glm::vec4& color = glm::vec4(1.0f),
                Font* font = nullptr);

    /** Update the text. */
    void SetText(const std::string& text) {
        m_text = text;
    }

    void Render(QuadShapes& qb) const override;

   private:
    std::string m_text;
    glm::vec4 m_color;
    Font* m_font;
};

}  // namespace gr
