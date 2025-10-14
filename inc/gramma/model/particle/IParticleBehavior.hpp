#pragma once

#include <gramma/core/SpatialGrid.hpp>

namespace gr {

class Particle;

class IParticleBehavior {
   public:
    virtual ~IParticleBehavior() = default;

    virtual void Update(Particle& p, float dt, const SpatialGrid<Particle*>& grid) const = 0;
};

}  // namespace gr
