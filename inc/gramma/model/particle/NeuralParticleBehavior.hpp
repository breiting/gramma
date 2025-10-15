
#pragma once
#include <gramma/model/evolution/Genome.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>

namespace gr {

class NeuralParticleBehavior : public IParticleBehavior {
   public:
    NeuralParticleBehavior(float radius);
    void Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const override;

   private:
    float m_Radius;
};

}  // namespace gr
