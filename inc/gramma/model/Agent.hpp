#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/AgentTraits.hpp>

namespace gr {

/** Represents an agent (person) in the evacuation simulation. */
struct Agent {
    glm::vec2 Pos;        // Current position (meters)
    glm::vec2 Vel;        // Current velocity (m/s)
    AgentTraits traits;   // Agent traits (age, radii, speed)
    int chosenExitIndex;  // Index of chosen exit (-1 if none)
};

}  // namespace gr