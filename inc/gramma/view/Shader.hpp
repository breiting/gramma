#pragma once
#include <glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>

namespace gr {

/** Minimal shader wrapper (unlit). */
class Shader {
   public:
    Shader() = default;
    ~Shader();

    bool Build(const std::string& vs, const std::string& fs);
    void Bind() const {
        glUseProgram(m_id);
    }
    GLuint Id() const {
        return m_id;
    }

    // convenience setters
    void SetMat4(const char* name, const glm::mat4& m) const;
    void SetVec3(const char* name, const glm::vec3& v) const;
    void SetFloat(const char* name, float v) const;

    // builds a tiny unlit shader
    bool BuildUnlit();

   private:
    GLuint m_id = 0;
};

}  // namespace gr
