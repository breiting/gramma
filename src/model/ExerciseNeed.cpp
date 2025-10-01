#include <algorithm>
#include <gramma/model/ExerciseNeed.hpp>

namespace gr {

ExerciseNeed::ExerciseNeed(float rate) : m_Value(0.0f), m_Rate(rate) {
}

void ExerciseNeed::Update(float dt) {
    // By default, Need rises slowly over time
    m_Value = std::min(1.0f, m_Value + m_Rate * dt);
}

float ExerciseNeed::Priority() const {
    return m_Value;
}

std::string ExerciseNeed::Name() const {
    return "Exercise";
}

void ExerciseNeed::Reset() {
    m_Value = 0.0f;
}

void ExerciseNeed::Increase(float dt) {
    m_Value = std::min(1.0f, m_Value + m_Rate * dt);
}

void ExerciseNeed::Decrease(float dt) {
    m_Value = std::max(0.0f, m_Value - m_Rate * dt * 3.0f);  // faster decrease while moving
}

}  // namespace gr
