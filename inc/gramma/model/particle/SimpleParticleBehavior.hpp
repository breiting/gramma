#pragma once
#include <glm/vec2.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>
#include <gramma/model/particle/Particle.hpp>

namespace gr {

class SimpleParticleBehavior : public IParticleBehavior {
   public:
    SimpleParticleBehavior(float radius, float strength);

    void Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const;

   private:
    float m_Radius;
    float m_Strength;
    float m_MaxSpeed = 25.0f;
};

}  // namespace gr
