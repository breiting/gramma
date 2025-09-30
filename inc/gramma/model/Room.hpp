#pragma once
#include <glm/vec2.hpp>

namespace gr {

/** Represents a room in the evacuation simulation. */
struct Room {
    glm::vec2 Pos;   // Bottom-left position of the room (meters)
    glm::vec2 Size;  // Size of the room (width, height in meters)
};

}  // namespace gr