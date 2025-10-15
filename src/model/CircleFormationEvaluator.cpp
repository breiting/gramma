#include "gramma/model/evolution/CircleFormationEvaluator.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <gramma/model/evolution/Genome.hpp>

namespace gr {

CircleFormationEvaluator::CircleFormationEvaluator(float radius) : m_Radius(radius) {
}

void CircleFormationEvaluator::Evaluate(ParticleSystem& system) {
    for (auto& p : system.GetParticles()) {
        glm::vec2 pos = p->GetPosition();
        float r = glm::length(pos);
        p->GetGenome()->SetFitness(1.0f / (1.0f + fabsf(r - m_Radius)));
    }
}

}  // namespace gr
