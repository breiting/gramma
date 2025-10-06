#include <glm/glm.hpp>
#include <gramma/model/EnergyNeed.hpp>

namespace gr {

EnergyNeed::EnergyNeed(const Params& p) : m_P(p) {
}

void EnergyNeed::Update(float dt) {
    // Grundverbrauch
    m_E -= m_P.basal * dt;
    m_E = glm::clamp(m_E, 0.0f, 1.0f);
}

float EnergyNeed::Priority() const {
    return 1.0f - m_E;  // je niedriger Energie, desto dringender
}

std::string EnergyNeed::Name() const {
    return "Energy";
}

void EnergyNeed::Reset() {
    // not used
}

void EnergyNeed::AddIntake(float e) {
    m_E = glm::clamp(m_E + e, 0.0f, 1.0f);
}

void EnergyNeed::AddRest(float dt) {
    m_E = glm::clamp(m_E + m_P.restGain * dt, 0.0f, 1.0f);
}

void EnergyNeed::AddActivityCost(float speed, float dt) {
    float cost = m_P.kActivity * std::pow(speed, m_P.p) * dt;
    m_E = glm::clamp(m_E - cost, 0.0f, 1.0f);
}

}  // namespace gr
