#include <glm/glm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/model/particle/ParticleLifeBehavior.hpp>

namespace gr {

ParticleLifeBehavior::ParticleLifeBehavior(float radius, const std::vector<std::vector<float>>& matrix)
    : m_Radius(radius), m_AttractionMatrix(matrix) {
}

void ParticleLifeBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const {
    const glm::vec2& pos = self.GetPosition();
    const int group = self.GetGroup();

    glm::vec2 force(0.0f);

    const float maxForce = 5.0f;
    const float minDist2 = 2.0f * 2.0f;         // squared
    const float radius2 = m_Radius * m_Radius;  // squared

    auto neighbors = grid.QueryNeighborhood(pos, m_Radius);

    for (const auto* other : neighbors) {
        if (other == &self) continue;

        glm::vec2 delta = other->GetPosition() - pos;
        float dist2 = glm::dot(delta, delta);  // faster than length²

        if (dist2 < 0.0001f || dist2 > radius2) continue;

        float dist = std::sqrt(dist2);
        glm::vec2 dir = delta / dist;  // normalized

        float interaction = m_AttractionMatrix[group][other->GetGroup()];

        if (group == other->GetGroup() && dist2 < minDist2) {
            float repelStrength = (std::sqrt(minDist2) - dist) / std::sqrt(minDist2);
            force -= dir * repelStrength * 2.5f;
        } else {
            float falloff = 1.0f - (dist / m_Radius);
            force += dir * interaction * falloff;
        }
    }

    float forceLen2 = glm::dot(force, force);
    if (forceLen2 > maxForce * maxForce) {
        force = glm::normalize(force) * maxForce;
    }

    // Boundary correction
    glm::vec2 correctedPos = pos + force * dt;
    glm::vec2 min = grid.GetMinBounds();
    glm::vec2 max = grid.GetMaxBounds();

    for (int i = 0; i < 2; ++i) {
        if (correctedPos[i] < min[i] + 1.0f) {
            force[i] += (min[i] + 1.0f - correctedPos[i]) * 5.0f;
        } else if (correctedPos[i] > max[i] - 1.0f) {
            force[i] -= (correctedPos[i] - (max[i] - 1.0f)) * 5.0f;
        }
    }

    glm::vec2 newVel = self.GetVelocity() + force * dt;
    // newVel *= 0.98f;  // damping
    self.SetVelocity(newVel);
}
}  // namespace gr
