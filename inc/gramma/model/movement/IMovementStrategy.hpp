#pragma once
#include <glm/vec2.hpp>

namespace gr {

class Agent;

/**
 * Interface for movement strategies (how an agent moves toward a target).
 */
class IMovementStrategy {
   public:
    virtual ~IMovementStrategy() = default;

    /**
     * Update the agent's movement towards the target.
     * @param agent The agent to move.
     * @param target The target position.
     * @param dt Delta time in seconds.
     */
    virtual void Update(Agent& agent, const glm::vec2& target, float dt) = 0;
};

}  // namespace gr
