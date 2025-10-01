#include <algorithm>
#include <gramma/model/FoodSource.hpp>

namespace gr {

FoodSource::FoodSource(const glm::vec2& pos, float nutrition, float regenRate)
    : m_Position(pos), m_Nutrition(nutrition), m_RegenRate(regenRate) {
}

const glm::vec2& FoodSource::GetPosition() const {
    return m_Position;
}

float FoodSource::Consume(float amount) {
    float eaten = std::min(amount, m_Nutrition);
    m_Nutrition -= eaten;
    return eaten;
}

void FoodSource::Regenerate(float dt) {
    m_Nutrition = std::min(1.0f, m_Nutrition + m_RegenRate * dt);
}

float FoodSource::GetNutrition() const {
    return m_Nutrition;
}

}  // namespace gr
