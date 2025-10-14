#include <glm/glm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/model/particle/ParticleLifeBehavior.hpp>

namespace gr {

float GetForce(float x, float r, float rMax, float maxForce) {
    if (x <= 0.0f) return -1.0f;

    if (x <= r) {
        float t = 1.0f - (x / r);                  // [0, 1]
        return -glm::max(2.0f, maxForce) * t * t;  // quadratic repulsion
    }

    float mid = 0.5f * (r + rMax);

    if (x <= mid) {
        return maxForce * (x - r) / (mid - r);
    }

    if (x <= rMax) {
        return maxForce * (1.0f - (x - mid) / (rMax - mid));
    }

    return 0.0f;
}

ParticleLifeBehavior::ParticleLifeBehavior(float radius, const std::vector<std::vector<float>>& matrix)
    : m_Radius(radius), m_AttractionMatrix(matrix) {
}

void ParticleLifeBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const {
    const glm::vec2& pos = self.GetPosition();
    int group = self.GetGroup();

    glm::vec2 force(0.0f);
    float r = 1.2;
    float rMax = m_Radius;  // Maximaler Interaktionsradius

    auto neighbors = grid.QueryNeighborhood(pos, rMax);

    for (const auto* other : neighbors) {
        if (other == &self) continue;

        glm::vec2 delta = other->GetPosition() - pos;
        float dist = glm::length(delta);
        if (dist < 0.001f || dist > rMax) continue;

        glm::vec2 dir = delta / dist;

        int g1 = group;
        int g2 = other->GetGroup();

        float maxForce = m_AttractionMatrix[g1][g2];
        float f = GetForce(dist, r, rMax, maxForce) * 10.0f;

        force += dir * f;
    }

    // --- Velocity integration ---
    glm::vec2 vel = self.GetVelocity();
    vel += force * dt;
    vel *= 0.95f;  // Friction / Damping

    // --- Boundary correction ---
    glm::vec2 nextPos = pos + vel * dt;
    glm::vec2 min = grid.GetMinBounds();
    glm::vec2 max = grid.GetMaxBounds();

    for (int i = 0; i < 2; ++i) {
        if (nextPos[i] < min[i] + r) {
            vel[i] += (min[i] + r - nextPos[i]) * 10.0f;
        } else if (nextPos[i] > max[i] - r) {
            vel[i] -= (nextPos[i] - (max[i] - r)) * 10.0f;
        }
    }

    self.SetVelocity(vel);
}

}  // namespace gr
