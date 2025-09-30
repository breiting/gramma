#pragma once
#include <glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <map>
#include <vector>

namespace gr {

class Shader;

/** Batch for drawing textured quads. */
class TexturedQuadShapes {
   public:
    TexturedQuadShapes() = default;
    ~TexturedQuadShapes();

    void Init();
    void Clear();
    /**
     * Add a textured quad batch entry.
     * pos: screen position, size: quad dimensions,
     * uvRect: (u0,v0,u1,v1), color: vertex tint,
     * tex: texture ID.
     */
    void AddQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& uvRect, const glm::vec4& color,
                 GLuint tex);
    void Upload();
    void Draw(const Shader& coloredShader, const Shader& texturedShader, const glm::mat4& vp,
              const glm::vec4& color = glm::vec4(1.0f));

   private:
    GLuint m_Vao = 0, m_Vbo = 0;
    std::map<GLuint, std::vector<float>> m_VerticesPerTex;
    std::map<GLuint, size_t> m_QuadCountPerTex;
};

}  // namespace gr
