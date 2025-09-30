#pragma once
#include <glm/vec2.hpp>

namespace gr {

/** Represents an exit in the evacuation simulation. */
struct Exit {
    glm::vec2 Pos;   // Center position of the exit (meters)
    glm::vec2 Size;  // Size of the exit (width, height in meters)
};

}  // namespace gr