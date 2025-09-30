#pragma once
#include <glm/vec2.hpp>
#include <vector>

namespace gr {

class Agent;

/** Interface for collision handling strategies in the evacuation simulation. */
class ICollisionHandler {
   public:
    virtual ~ICollisionHandler() = default;

    /** Adjust velocities of agents to handle collisions. */
    virtual void HandleCollisions(std::vector<Agent>& agents) const = 0;
};

}  // namespace gr