#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Exit.hpp>
#include <vector>

namespace gr {

/** Represents a room in the evacuation simulation. */
struct Room {
    glm::vec2 Position;       // Bottom-left position of the room (meters)
    glm::vec2 Size;           // Size of the room (width, height in meters)
    std::vector<Exit> Exits;  // Exits in the room
};

}  // namespace gr
