#pragma once

#include <glm/vec2.hpp>
#include <gramma/model/particle/ParticleSystem.hpp>
#include <vector>

namespace gr {

/**
 * @class CircleFormationEvaluator
 * @brief Evaluates how closely particles match a target circular arrangement.
 */
class CircleFormationEvaluator {
   public:
    CircleFormationEvaluator(float radius, glm::vec2 center);

    /**
     * @brief Evaluate particle system and return fitness for each particle.
     */
    std::vector<float> Evaluate(const ParticleSystem& system) const;

   private:
    float m_TargetRadius;
    glm::vec2 m_Center;
};

}  // namespace gr
