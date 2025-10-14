#include <glm/glm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <gramma/model/particle/ParticleLifeBehavior.hpp>

namespace gr {

ParticleLifeBehavior::ParticleLifeBehavior(float radius, const std::vector<std::vector<float>>& matrix)
    : m_Radius(radius), m_AttractionMatrix(matrix) {
}

void ParticleLifeBehavior::Update(Particle& p, float dt, const SpatialGrid<Particle*>& grid) const {
    glm::vec2 force{0.0f};

    auto neighbors = grid.QueryNeighborhood(p.GetPosition(), m_Radius);
    for (const auto* other : neighbors) {
        if (other == &p) continue;

        glm::vec2 diff = other->GetPosition() - p.GetPosition();
        float dist = glm::length(diff);
        if (dist < 0.001f || dist > m_Radius) continue;

        glm::vec2 dir = glm::normalize(diff);
        int g1 = p.GetGroup();
        int g2 = other->GetGroup();
        float attraction = m_AttractionMatrix[g1][g2];

        // Inversely proportional to distance
        float strength = attraction * (1.0f - dist / m_Radius);
        force += dir * strength;
    }

    glm::vec2 newVel = force;
    p.SetVelocity(newVel);
    p.SetPosition(p.GetPosition() + newVel * dt);
}

}  // namespace gr
