#include <tbb/parallel_for.h>

#include <cmath>
#include <gramma/model/evolution/CircleFormationEvaluator.hpp>

namespace gr {

CircleFormationEvaluator::CircleFormationEvaluator(float radius, glm::vec2 center)
    : m_TargetRadius(radius), m_Center(center) {
}

std::vector<float> CircleFormationEvaluator::Evaluate(const ParticleSystem& system) const {
    const auto& parts = system.GetParticles();
    std::vector<float> fitness(parts.size(), 0.0f);

    tbb::parallel_for(size_t(0), parts.size(), [&](size_t i) {
        const auto& p = *parts[i];
        glm::vec2 pos = p.GetPosition() - m_Center;
        float dist = glm::length(pos);

        // Fitness: 1.0 if inside the circle, falls off outside
        if (dist <= m_TargetRadius) {
            float t = dist / m_TargetRadius;
            fitness[i] = 1.0f - t * 0.5f;  // slight reward gradient (center best)
        } else {
            float overshoot = dist - m_TargetRadius;
            fitness[i] = 1.0f / (1.0f + overshoot);  // quickly penalize outside
        }
    });

    return fitness;
}

}  // namespace gr
