#pragma once

#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>

namespace gr {

class RepulsionOnlyBehavior : public IParticleBehavior {
   public:
    explicit RepulsionOnlyBehavior(float radius, float strength = 5.0f, float damping = 0.90f);

    void Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const override;

   private:
    float m_Radius;
    float m_Strength;
    float m_Damping;
};

}  // namespace gr
