#pragma once

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

/**
 * @brief GPU instanced renderer for a collection of particles.
 *
 * Call Init() once to create OpenGL buffers, then UpdateInstances() to upload
 * particle transforms each frame prior to Draw().
 */
class ParticleView {
   public:
    ParticleView();
    ~ParticleView();

    /** @brief Lazily create shader program and instance buffers. */
    void Init();
    /**
     * @brief Upload per-particle instance data to the GPU.
     * @param particles Source particle collection owned by the particle system.
     */
    void UpdateInstances(const std::vector<std::unique_ptr<Particle>>& particles);
    /**
     * @brief Render the batched particles using the given view-projection matrix.
     */
    void Draw(const glm::mat4& vp);

   private:
    struct InstanceData {
        glm::vec2 position;
        float radius;
        glm::vec4 color;
    };

    GLuint m_Vao = 0;
    GLuint m_Vbo = 0;
    Shader m_Shader;

    std::vector<InstanceData> m_InstanceData;
};

}  // namespace gr
