#include <gramma/model/particle/Particle.hpp>

namespace gr {

Particle::Particle(std::string id, const glm::vec2& position, int group, float radius)
    : m_Id(std::move(id)), m_Position(position), m_Group(group), m_Radius(radius) {
}

const std::string& Particle::GetId() const {
    return m_Id;
}

const glm::vec2& Particle::GetPosition() const {
    return m_Position;
}

void Particle::SetPosition(const glm::vec2& pos) {
    m_Position = pos;
}

const glm::vec2& Particle::GetVelocity() const {
    return m_Velocity;
}

void Particle::SetVelocity(const glm::vec2& vel) {
    m_Velocity = vel;
}

float Particle::GetRadius() const {
    return m_Radius;
}

Genome* Particle::GetGenome() {
    return m_Genome;
}

void Particle::SetGenome(Genome* gen) {
    m_Genome = gen;
}

int Particle::GetGroup() const {
    return m_Group;
}

}  // namespace gr
