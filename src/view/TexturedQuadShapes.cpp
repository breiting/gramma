#include <gramma/view/Shader.hpp>
#include <gramma/view/TexturedQuadShapes.hpp>

namespace gr {

TexturedQuadShapes::~TexturedQuadShapes() {
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
}

void TexturedQuadShapes::Init() {
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    // pos x,y (2), uv u,v (2), color r,g,b,a (4) = 8 floats per vertex, 6 vertices per quad
    const GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void TexturedQuadShapes::Clear() {
    m_VerticesPerTex.clear();
    m_QuadCountPerTex.clear();
}

void TexturedQuadShapes::AddQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& uvRect,
                                 const glm::vec4& color, GLuint tex) {
    auto& verts = m_VerticesPerTex[tex];
    auto& count = m_QuadCountPerTex[tex];
    // uvRect: u0, v0, u1, v1
    float u0 = uvRect.x, v0 = uvRect.y, u1 = uvRect.z, v1 = uvRect.w;
    // Triangle 1
    // BL
    verts.insert(verts.end(), {pos.x, pos.y, u0, v1, color.r, color.g, color.b, color.a});
    // BR
    verts.insert(verts.end(), {pos.x + size.x, pos.y, u1, v1, color.r, color.g, color.b, color.a});
    // TR
    verts.insert(verts.end(), {pos.x + size.x, pos.y + size.y, u1, v0, color.r, color.g, color.b, color.a});
    // Triangle 2
    // BL
    verts.insert(verts.end(), {pos.x, pos.y, u0, v1, color.r, color.g, color.b, color.a});
    // TR
    verts.insert(verts.end(), {pos.x + size.x, pos.y + size.y, u1, v0, color.r, color.g, color.b, color.a});
    // TL
    verts.insert(verts.end(), {pos.x, pos.y + size.y, u0, v0, color.r, color.g, color.b, color.a});
    count++;
}

void TexturedQuadShapes::Upload() {
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    // total size is sum of all vertices
    std::vector<float> all;
    for (auto& kv : m_VerticesPerTex) {
        auto& v = kv.second;
        all.insert(all.end(), v.begin(), v.end());
    }
    glBufferData(GL_ARRAY_BUFFER, all.size() * sizeof(float), all.data(), GL_DYNAMIC_DRAW);
}

void TexturedQuadShapes::Draw(const Shader& coloredShader, const Shader& texturedShader, const glm::mat4& vp,
                              float alpha) {
    if (m_VerticesPerTex.empty()) return;
    bool hasTextures = false;
    for (auto& kv : m_VerticesPerTex) {
        if (kv.first != 0) {
            hasTextures = true;
            break;
        }
    }
    const Shader& shader = hasTextures ? texturedShader : coloredShader;
    shader.Bind();
    shader.SetMat4("uMVP", vp);
    if (hasTextures) {
        shader.SetBool("uUseTexture", true);
    } else {
        shader.SetBool("uUseTexture", false);
    }
    shader.SetFloat("uAlpha", alpha);
    glBindVertexArray(m_Vao);
    size_t offset = 0;
    for (auto& kv : m_VerticesPerTex) {
        GLuint tex = kv.first;
        size_t count = m_QuadCountPerTex[tex];
        if (count == 0) continue;
        if (tex != 0) {
            glBindTexture(GL_TEXTURE_2D, tex);
            shader.SetInt("uTexture", 0);
        }
        glDrawArrays(GL_TRIANGLES, static_cast<GLint>(offset), static_cast<GLsizei>(count * 6));
        offset += count * 6;
    }
}

}  // namespace gr
