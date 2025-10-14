#pragma once

#include <gramma/core/SpatialGrid.hpp>

namespace gr {

class Particle;

class ParticleBehavior {
   public:
    virtual ~ParticleBehavior() = default;

    virtual void Update(Particle& p, float dt, const SpatialGrid<Particle*>& grid) const = 0;
};

}  // namespace gr
