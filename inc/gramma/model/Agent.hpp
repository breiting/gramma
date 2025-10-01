#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/AgentTraits.hpp>

namespace gr {

/** Represents an agent (person) in the evacuation simulation. */
struct Agent {
    glm::vec2 Position;   // Current position (meters)
    glm::vec2 Velocity;   // Current velocity (m/s)
    float Heading;        // Orientation (0 = north, 90 = east, ...)
    AgentTraits Traits;   // Agent traits (age, speed, ...)
    int chosenExitIndex;  // TODO
};

}  // namespace gr
