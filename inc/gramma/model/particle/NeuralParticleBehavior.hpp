#pragma once

#include <Eigen/Dense>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>
#include <gramma/model/particle/Particle.hpp>

namespace gr {

/**
 * @brief Neural network–based particle behavior with local repulsion and speed limiting.
 */
class NeuralParticleBehavior : public IParticleBehavior {
   public:
    NeuralParticleBehavior(float radius, float repulsionRadius = 2.0f, float repulsionStrength = 1.0f,
                           float maxSpeed = 8.0f, float forceScale = 10.0f);

    void Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const override;

   private:
    float m_Radius;
    float m_RepulsionRadius;
    float m_RepulsionStrength;
    float m_MaxSpeed;
    float m_ForceScale;
};

}  // namespace gr
