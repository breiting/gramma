#include <gramma/view/Shader.hpp>
#include <iostream>

namespace gr {

static GLuint compile(GLenum type, const std::string& src) {
    GLuint s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(s, 2048, nullptr, log);
        throw std::runtime_error(std::string("Shader compilation failed: ") + log);
    }
    return s;
}
static GLuint link(GLuint vs, GLuint fs) {
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(p, 2048, nullptr, log);
        glDeleteShader(vs);
        glDeleteShader(fs);
        throw std::runtime_error(std::string("Shader linking failed: ") + log);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

Shader::~Shader() {
    if (m_id) glDeleteProgram(m_id);
}

void Shader::Build(const std::string& vs, const std::string& fs) {
    GLuint v = compile(GL_VERTEX_SHADER, vs);
    GLuint f = compile(GL_FRAGMENT_SHADER, fs);
    m_id = link(v, f);
    if (!m_id) {
        throw std::runtime_error("Failed to create shader program");
    }
}

void Shader::SetMat4(const char* name, const glm::mat4& m) const {
    GLint loc = glGetUniformLocation(m_id, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}
void Shader::SetVec3(const char* name, const glm::vec3& v) const {
    GLint loc = glGetUniformLocation(m_id, name);
    glUniform3fv(loc, 1, &v[0]);
}
void Shader::SetFloat(const char* name, float v) const {
    GLint loc = glGetUniformLocation(m_id, name);
    glUniform1f(loc, v);
}

void Shader::BuildUnlit() {
    const std::string vs = R"GLSL(
    #version 330 core
    layout(location=0) in vec2 inPos;
    uniform mat4 uMVP;
    void main(){
      gl_Position = uMVP * vec4(inPos,0,1);
    }
  )GLSL";
    const std::string fs = R"GLSL(
    #version 330 core
    out vec4 fragColor;
    uniform vec3 uColor;
    void main(){ fragColor = vec4(uColor,1.0); }
  )GLSL";
    Build(vs, fs);
}

}  // namespace gr
