#pragma once
#include <glm/vec2.hpp>

namespace gr {

/** Represents an agent (person) in the evacuation simulation. */
struct Agent {
    glm::vec2 Pos;    // Current position (meters)
    glm::vec2 Vel;    // Current velocity (m/s)
    float Radius;     // Agent radius (meters)
    float SpeedPref;  // Preferred speed (m/s)
};

}  // namespace gr