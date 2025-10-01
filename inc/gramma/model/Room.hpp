#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Exit.hpp>
#include <vector>

namespace gr {

/** Represents a room in the evacuation simulation. */
class Room {
   public:
    Room(const std::vector<glm::vec2>& contour);

    const std::vector<glm::vec2>& GetContour() const;

   private:
    std::vector<glm::vec2> m_Contour;
    float m_Thickness;
    std::vector<Exit> m_Exits;  // Exits in the room
};

}  // namespace gr
