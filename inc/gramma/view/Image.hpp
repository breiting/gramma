#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <gramma/view/IRenderable.hpp>
#include <gramma/view/Texture.hpp>
#include <string>

#include "gramma/view/TexturedQuadShapes.hpp"

namespace gr {

/** Image for displaying textures. */
class Image : public IRenderable {
   public:
    Image(const glm::vec2& pos, const glm::vec2& size, const std::string& path,
          const glm::vec4& color = glm::vec4(1.0f));

    void Render(TexturedQuadShapes& qb) const override;

   private:
    Texture m_Texture;
    glm::vec2 m_Size;
    glm::vec4 m_Color;
};

}  // namespace gr
