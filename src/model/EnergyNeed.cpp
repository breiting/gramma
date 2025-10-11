#include <algorithm>
#include <glm/glm.hpp>
#include <gramma/model/need/EnergyNeed.hpp>

namespace gr {

EnergyNeed::EnergyNeed(const EnergyNeedParams& p) : m_P(p) {
}

void EnergyNeed::Update(float dt) {
    m_E = std::max(0.0f, m_E - m_P.basal * dt);
}

float EnergyNeed::Priority() const {
    return 1.0f - m_E;
}

void EnergyNeed::AddIntake(float e) {
    m_E = glm::clamp(m_E + e, 0.0f, 1.0f);
}

void EnergyNeed::AddRest(float dt) {
    m_E = glm::clamp(m_E + m_P.restGain * dt, 0.0f, 1.0f);
}

void EnergyNeed::AddActivityCost(float speed, float dt) {
    float cost = m_P.kActivity * std::pow(speed, m_P.p) * dt;
    m_E = std::max(0.0f, m_E - cost);
}

}  // namespace gr
