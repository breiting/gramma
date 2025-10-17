#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <gramma/model/evolution/FeedForwardGenome.hpp>
#include <gramma/model/particle/NeuralParticleBehavior.hpp>

namespace gr {

NeuralParticleBehavior::NeuralParticleBehavior(float radius, float repulsionRadius, float repulsionStrength,
                                               float maxSpeed, float forceScale)
    : m_Radius(radius),
      m_RepulsionRadius(repulsionRadius),
      m_RepulsionStrength(repulsionStrength),
      m_MaxSpeed(maxSpeed),
      m_ForceScale(forceScale) {
}

void NeuralParticleBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const {
    auto* g = self.GetGenome();
    if (!g) return;

    glm::vec2 pos = self.GetPosition();
    glm::vec2 vel = self.GetVelocity();

    // --- Neural movement force ---
    std::vector<float> input = {pos.x / m_Radius, pos.y / m_Radius, vel.x * 0.1f, vel.y * 0.1f};
    auto out = g->Forward(input);

    glm::vec2 neuralForce(out[0], out[1]);
    neuralForce *= m_ForceScale;  // amplify neural output

    // --- Local repulsion from nearby particles ---
    glm::vec2 repulsion(0.0f);
    auto neighbors = grid.QueryNeighborhood(pos, m_RepulsionRadius);

    for (const auto* other : neighbors) {
        if (other == &self) continue;

        glm::vec2 delta = pos - other->GetPosition();
        float dist2 = glm::dot(delta, delta);
        if (dist2 < 0.0001f || dist2 > m_RepulsionRadius * m_RepulsionRadius) continue;

        float dist = std::sqrt(dist2);
        glm::vec2 dir = delta / dist;
        float strength = (m_RepulsionRadius - dist) / m_RepulsionRadius;
        repulsion += dir * strength * m_RepulsionStrength;
    }

    // --- Combine forces ---
    glm::vec2 totalForce = neuralForce + repulsion;

    // Integrate velocity
    vel += totalForce * dt;

    // --- Clamp max speed ---
    float speed2 = glm::dot(vel, vel);
    if (speed2 > m_MaxSpeed * m_MaxSpeed) {
        vel = glm::normalize(vel) * m_MaxSpeed;
    }

    // --- Damping ---
    vel *= 0.98f;

    // --- Boundary correction ---
    glm::vec2 nextPos = pos + vel * dt;
    glm::vec2 min = grid.GetMinBounds();
    glm::vec2 max = grid.GetMaxBounds();

    for (int i = 0; i < 2; ++i) {
        if (nextPos[i] < min[i] + 1.0f) {
            vel[i] += (min[i] + 1.0f - nextPos[i]) * 10.0f;
        } else if (nextPos[i] > max[i] - 1.0f) {
            vel[i] -= (nextPos[i] - (max[i] - 1.0f)) * 10.0f;
        }
    }

    self.SetVelocity(vel);
}

}  // namespace gr
