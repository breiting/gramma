#pragma once
#include <glm/glm.hpp>
#include <gramma/model/particle/ParticleBehavior.hpp>
#include <vector>

namespace gr {

class ParticleLifeBehavior : public ParticleBehavior {
   public:
    ParticleLifeBehavior(float radius, const std::vector<std::vector<float>>& matrix);

    void Update(Particle& p, float dt, const SpatialGrid<Particle*>& grid) const override;

   private:
    float m_Radius;
    std::vector<std::vector<float>> m_AttractionMatrix;
};

}  // namespace gr
