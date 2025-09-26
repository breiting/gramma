#include <glad.h>

#include <gramma/LineBatch.hpp>
#include <iostream>

namespace gramma {
static GLuint mkShader(GLenum t, const char* s) {
    GLuint sh = glCreateShader(t);
    glShaderSource(sh, 1, &s, nullptr);
    glCompileShader(sh);
    GLint ok;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(sh, 1024, nullptr, log);
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
        char log[1024];
        glGetProgramInfoLog(p, 1024, nullptr, log);
        std::cerr << log << "\n";
    }
    glDeleteShader(v);
    glDeleteShader(f);
    return p;
}

bool LineBatch::init() {
    const char* VS = R"GLSL(
    #version 330 core
    layout(location=0) in vec2 inPos;
    uniform mat4 uVP;
    void main(){ gl_Position = uVP * vec4(inPos,0,1); }
  )GLSL";
    const char* FS = R"GLSL(
    #version 330 core
    out vec4 fragColor; uniform float uAlpha;
    void main(){ fragColor = vec4(1.0,1.0,1.0,uAlpha); }
  )GLSL";
    m_prog = mkProg(VS, FS);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return true;
}

void LineBatch::set(const std::vector<glm::vec2>& pts) {
    m_count = (int)pts.size();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_count * 2 * sizeof(float), pts.data(), GL_DYNAMIC_DRAW);
}

void LineBatch::draw(const glm::mat4& vp, float alpha) const {
    if (m_count <= 0) return;
    glUseProgram(m_prog);
    glUniformMatrix4fv(glGetUniformLocation(m_prog, "uVP"), 1, GL_FALSE, &vp[0][0]);
    glUniform1f(glGetUniformLocation(m_prog, "uAlpha"), alpha);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, m_count);
}

}  // namespace gramma
