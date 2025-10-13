#pragma once
#include <gramma/model/movement/IMovementStrategy.hpp>

namespace gr {

/**
 * Simple direct movement: move in a straight line towards the target.
 */
class DirectMovement : public IMovementStrategy {
   public:
    void Update(IPhysicsEngine* engine, Agent& agent, const glm::vec2& target, float dt) override;

   private:
    float m_BlockTimer = 0.0f;
    float m_LastDist = std::numeric_limits<float>::max();
};

}  // namespace gr
