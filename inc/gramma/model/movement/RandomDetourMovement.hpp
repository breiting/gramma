#pragma once
#include <gramma/model/movement/IMovementStrategy.hpp>

namespace gr {

/**
 * Movement with random perturbations (like a noisy walk).
 */
class RandomDetourMovement : public IMovementStrategy {
   public:
    void Update(IPhysicsEngine* engine, Agent& agent, const glm::vec2& target, float dt) override;
};

}  // namespace gr
