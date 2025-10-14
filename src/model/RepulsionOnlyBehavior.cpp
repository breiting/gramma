#include <glm/glm.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/model/particle/RepulsionOnlyBehavior.hpp>

namespace gr {

RepulsionOnlyBehavior::RepulsionOnlyBehavior(float radius, float strength, float damping)
    : m_Radius(radius), m_Strength(strength), m_Damping(damping) {
}

void RepulsionOnlyBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const {
    const glm::vec2& pos = self.GetPosition();
    glm::vec2 force(0.0f);

    auto neighbors = grid.QueryNeighborhood(pos, m_Radius);

    for (const auto* other : neighbors) {
        if (other == &self) continue;

        glm::vec2 delta = other->GetPosition() - pos;
        float dist = glm::length(delta);
        if (dist < 0.01f || dist > m_Radius) continue;

        glm::vec2 dir = glm::normalize(delta);
        float falloff = (m_Radius - dist) / m_Radius;

        force -= dir * falloff * m_Strength;
    }

    glm::vec2 vel = self.GetVelocity();
    vel += force * dt;
    vel *= m_Damping;

    self.SetVelocity(vel);
}

}  // namespace gr
