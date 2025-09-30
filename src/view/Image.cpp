#include <gramma/view/Image.hpp>
#include <gramma/view/TexturedQuadShapes.hpp>

namespace gr {

Image::Image(const glm::vec2& pos, const glm::vec2& size, const std::string& path, const glm::vec4& color)
    : Element(pos), m_Size(size), m_Color(color) {
    m_Texture.Load(path);
}

void Image::Render(TexturedQuadShapes& qb) const {
    qb.AddQuad(m_Pos, m_Size, glm::vec4(0, 0, 1, 1), m_Color, m_Texture.Id());
}

}  // namespace gr