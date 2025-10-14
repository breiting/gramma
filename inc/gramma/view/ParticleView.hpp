#pragma once
#include <glm/mat4x4.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/view/Shader.hpp>
#include <memory>
#include <vector>

namespace gr {

/**
 * ParticleView:
 * All Particles with one draw call
 */
class ParticleView {
   public:
    ParticleView();
    ~ParticleView();

    void Init();
    void UpdateInstances(const std::vector<std::unique_ptr<Particle>>& particles);
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
