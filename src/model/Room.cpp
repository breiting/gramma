#include <gramma/model/Room.hpp>

namespace gr {

Room::Room(const std::vector<glm::vec2>& contour) : m_Contour(contour) {
}

const std::vector<glm::vec2>& Room::GetContour() const {
    return m_Contour;
}

}  // namespace gr
