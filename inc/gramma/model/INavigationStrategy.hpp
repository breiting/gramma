#pragma once
#include <glm/vec2.hpp>

namespace gr {

class Agent;
struct Exit;

/** Interface for navigation strategies in the evacuation simulation. */
class INavigationStrategy {
   public:
    virtual ~INavigationStrategy() = default;

    /** Compute the desired velocity for an agent towards the exit. */
    virtual glm::vec2 ComputeDesiredVelocity(const Agent& agent, const Exit& exit) const = 0;
};

}  // namespace gr