#include <algorithm>
#include <gramma/model/HungerNeed.hpp>

namespace gr {

HungerNeed::HungerNeed(float rate) : m_Value(0.0f), m_Rate(rate) {
}

void HungerNeed::Update(float dt) {
    m_Value = std::min(1.0f, m_Value + m_Rate * dt);
}

float HungerNeed::Priority() const {
    return m_Value;
}

std::string HungerNeed::Name() const {
    return "Hunger";
}

void HungerNeed::Reset() {
    m_Value = 0.0f;
}

}  // namespace gr
