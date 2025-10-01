#include <algorithm>
#include <gramma/model/FoodSource.hpp>

namespace gr {

FoodSource::FoodSource(const glm::vec2& pos, float nutrition) : m_Position(pos), m_Nutrition(nutrition) {
}

const glm::vec2& FoodSource::GetPosition() const {
    return m_Position;
}

float FoodSource::Consume() {
    float eaten = std::min(1.0f, m_Nutrition);
    m_Nutrition -= eaten;
    return eaten;
}

}  // namespace gr
