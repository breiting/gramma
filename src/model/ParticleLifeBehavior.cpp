#include <algorithm>
#include <stdexcept>

#include <glm/glm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/model/particle/ParticleLifeBehavior.hpp>

namespace gr {

namespace {

float ComputeForce(float distance, float innerRadius, float outerRadius, float maxForce) {
    if (distance <= 0.0f) {
        return -glm::max(2.0f, maxForce);
    }

    if (distance <= innerRadius) {
        const float t = 1.0f - (distance / innerRadius);  // [0, 1]
        return -glm::max(2.0f, maxForce) * t * t;         // quadratic repulsion
    }

    const float mid = 0.5f * (innerRadius + outerRadius);

    if (distance <= mid) {
        return maxForce * (distance - innerRadius) / (mid - innerRadius);
    }

    if (distance <= outerRadius) {
        return maxForce * (1.0f - (distance - mid) / (outerRadius - mid));
    }

    return 0.0f;
}

}  // namespace

ParticleLifeBehavior::ParticleLifeBehavior(float radius, const std::vector<std::vector<float>>& matrix)
    : m_Radius(radius), m_AttractionMatrix(matrix) {
    if (matrix.empty() || matrix.front().empty()) {
        throw std::invalid_argument("ParticleLifeBehavior requires a non-empty attraction matrix.");
    }

    const std::size_t expectedSize = matrix.front().size();
    if (matrix.size() != expectedSize) {
        throw std::invalid_argument("ParticleLifeBehavior expects a square attraction matrix.");
    }

    for (const auto& row : matrix) {
        if (row.size() != expectedSize) {
            throw std::invalid_argument("ParticleLifeBehavior received a jagged attraction matrix.");
        }
    }

    m_GroupCount = expectedSize;
    constexpr float kMinRadius = 0.001f;
    m_CoreRadius = std::clamp(radius * 0.6f, kMinRadius, radius);
}

void ParticleLifeBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>& grid) const {
    const glm::vec2& pos = self.GetPosition();
    const int group = self.GetGroup();
    if (group < 0 || static_cast<std::size_t>(group) >= m_GroupCount) {
        throw std::out_of_range("ParticleLifeBehavior encountered a particle with an invalid group id.");
    }

    glm::vec2 force(0.0f);
    const float innerRadius = m_CoreRadius;
    const float outerRadius = m_Radius;

    const auto neighbors = grid.QueryNeighborhood(pos, outerRadius);

    for (const auto* other : neighbors) {
        if (other == &self) continue;

        const glm::vec2 delta = other->GetPosition() - pos;
        const float dist = glm::length(delta);
        if (dist < 0.001f || dist > outerRadius) continue;

        const glm::vec2 dir = delta / dist;

        const auto g1 = static_cast<std::size_t>(group);
        const int neighborGroup = other->GetGroup();
        if (neighborGroup < 0 || static_cast<std::size_t>(neighborGroup) >= m_GroupCount) {
            throw std::out_of_range("ParticleLifeBehavior encountered a neighbor with an invalid group id.");
        }
        const auto g2 = static_cast<std::size_t>(neighborGroup);

        const float maxForce = m_AttractionMatrix[g1][g2];
        const float f = ComputeForce(dist, innerRadius, outerRadius, maxForce) * 10.0f;

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
        if (nextPos[i] < min[i] + innerRadius) {
            vel[i] += (min[i] + innerRadius - nextPos[i]) * 10.0f;
        } else if (nextPos[i] > max[i] - innerRadius) {
            vel[i] -= (nextPos[i] - (max[i] - innerRadius)) * 10.0f;
        }
    }

    self.SetVelocity(vel);
}

}  // namespace gr
