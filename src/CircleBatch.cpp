#include <glad.h>

#include <gramma/CircleBatch.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace gramma {

static GLuint mkShader(GLenum t, const char* s) {
    GLuint sh = glCreateShader(t);
    glShaderSource(sh, 1, &s, nullptr);
    glCompileShader(sh);
    GLint ok;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(sh, 2048, nullptr, log);
        std::cerr << log << "\n";
    }
    return sh;
}
static GLuint mkProg(const char* vs, const char* fs) {
    GLuint p = glCreateProgram();
    GLuint v = mkShader(GL_VERTEX_SHADER, vs), f = mkShader(GL_FRAGMENT_SHADER, fs);
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    GLint ok;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(p, 2048, nullptr, log);
        std::cerr << log << "\n";
    }
    glDeleteShader(v);
    glDeleteShader(f);
    return p;
}

bool CircleBatch::init() {
    const char* VS = R"GLSL(
    #version 330 core
    layout(location=0) in vec2 inLocal;       // quad corners in [-1,1]
    layout(location=1) in vec2 inPos;         // world position (m)
    layout(location=2) in float inSize;       // diameter (m)
    uniform mat4 uVP;
    out vec2 vLocal;
    void main(){
      vec2 world = inPos + 0.5 * inSize * inLocal;
      gl_Position = uVP * vec4(world, 0.0, 1.0);
      vLocal = inLocal;
    }
  )GLSL";
    const char* FS = R"GLSL(
    #version 330 core
    in vec2 vLocal;
    out vec4 fragColor;
    uniform float uAlpha;
    void main(){
      float r = length(vLocal);                 // quad is [-1,1] → radius = 1
      float a = smoothstep(1.0, 0.9, r);        // soft edge
      fragColor = vec4(1.0,1.0,1.0, a * uAlpha);
    }
  )GLSL";
    m_prog = mkProg(VS, FS);

    // quad geometry
    float quad[8] = {-1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f};
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboQuad);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_vboInst);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboInst);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // instances: pos (loc=1), size (loc=2)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)offsetof(Instance, diameter));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    return true;
}

void CircleBatch::clear() {
    m_data.clear();
}
void CircleBatch::add(const glm::vec2& pos, float diameter) {
    m_data.push_back({pos, diameter});
}
void CircleBatch::upload() {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboInst);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(Instance), m_data.data(), GL_DYNAMIC_DRAW);
}

void CircleBatch::draw(const glm::mat4& vp, float alpha) const {
    glUseProgram(m_prog);
    glUniformMatrix4fv(glGetUniformLocation(m_prog, "uVP"), 1, GL_FALSE, &vp[0][0]);
    glUniform1f(glGetUniformLocation(m_prog, "uAlpha"), alpha);
    glBindVertexArray(m_vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)m_data.size());
}

}  // namespace gramma
