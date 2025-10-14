#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>  // for length2()
#include <gramma/model/particle/SimpleParticleBehavior.hpp>
#include <iostream>

namespace gr {

SimpleParticleBehavior::SimpleParticleBehavior(float radius, float strength) : m_Radius(radius), m_Strength(strength) {
}

void SimpleParticleBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const {
    glm::vec2 pos = self.GetPosition();
    int group = self.GetGroup();

    glm::vec2 force(0.0f);
    auto neighbors = grid.QueryNeighborhood(pos, m_Radius);

    for (const Particle* other : neighbors) {
        if (other == &self) continue;

        glm::vec2 delta = other->GetPosition() - pos;
        float dist2 = glm::length2(delta);
        if (dist2 < 0.0001f || dist2 > m_Radius * m_Radius) continue;

        float dist = std::sqrt(dist2);
        dist = std::max(dist, 1.0f);  // prevent collapse
        glm::vec2 dir = delta / dist;

        float sign = (other->GetGroup() == group) ? +1.0f : -1.0f;
        float falloff = 1.0f - (dist / m_Radius);

        // Smooth, bounded force
        force += sign * dir * (m_Strength * falloff);

        // Short-range soft repulsion (avoids collapse)
        if (dist < 1.5f) force -= dir * (1.5f - dist) * 3.0f;
    }
    // Apply force to velocity
    glm::vec2 vel = self.GetVelocity() + force * dt;

    // Limit max speed
    if (glm::length2(vel) > m_MaxSpeed * m_MaxSpeed) {
        vel = glm::normalize(vel) * m_MaxSpeed;
    }

    glm::vec2 nextPos = pos + vel * dt;
    glm::vec2 min = grid.GetMinBounds();
    glm::vec2 max = grid.GetMaxBounds();
    for (int i = 0; i < 2; ++i) {
        if (nextPos[i] < min[i]) {
            nextPos[i] = min[i];
            vel[i] *= -0.5f;  // Dämpfung beim Abprallen
        } else if (nextPos[i] > max[i]) {
            nextPos[i] = max[i];
            vel[i] *= -0.5f;
        }
    }

    self.SetVelocity(vel);
}

}  // namespace gr
