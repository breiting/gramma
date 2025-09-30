#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Exit.hpp>
#include <vector>

namespace gr {

/** Represents a room in the evacuation simulation. */
struct Room {
    glm::vec2 Pos;            // Bottom-left position of the room (meters)
    glm::vec2 Size;           // Size of the room (width, height in meters)
    std::vector<Exit> exits;  // Exits in the room
};

}  // namespace gr