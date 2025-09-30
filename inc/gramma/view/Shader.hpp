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

    /** Build shader from source strings.
     * @param vs Vertex shader source.
     * @param fs Fragment shader source.
     * @throws std::runtime_error if compilation or linking fails.
     */
    void Build(const std::string& vs, const std::string& fs);

    /** Bind the shader. */
    void Bind() const {
        glUseProgram(m_id);
    }

    /** @return The OpenGL program ID. */
    GLuint Id() const {
        return m_id;
    }

    // convenience setters
    /** Set a mat4 uniform.
     * @param name Uniform name.
     * @param m Matrix value.
     */
    void SetMat4(const char* name, const glm::mat4& m) const;

    /** Set a vec3 uniform.
     * @param name Uniform name.
     * @param v Vector value.
     */
    void SetVec3(const char* name, const glm::vec3& v) const;

    /** Set a float uniform.
     * @param name Uniform name.
     * @param v Float value.
     */
    void SetFloat(const char* name, float v) const;

    /** Build a default unlit shader.
     * @throws std::runtime_error if build fails.
     */
    void BuildUnlit();

   private:
    GLuint m_id = 0;
};

}  // namespace gr
