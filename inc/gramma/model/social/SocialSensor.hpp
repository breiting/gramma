#pragma once

#include <glm/vec2.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <vector>

namespace gr {

/**
 * @brief The SocialSensor is responsible for perceiving nearby agents and
 *        estimating the most attractive (or repulsive) neighbor based on
 *        an attraction matrix (e.g., age similarity).
 *
 *        It provides static helper methods so that it can be used both
 *        by Needs and TaskBuilders without creating instances.
 */
class SocialSensor {
   public:
    SocialSensor() = default;

    /**
     * @brief Query all nearby agents within the social radius.
     */
    static std::vector<const Agent*> QueryNearbyAgents(const Agent& self, const Environment& env);

    /**
     * @brief Find the most attractive agent according to the attraction matrix.
     * @return A pointer to the most attractive nearby agent, or nullptr if none found.
     */
    static const Agent* FindMostAttractive(const Agent& self, const Environment& env);

    /**
     * @brief Compute the average social attraction score of the surroundings.
     * This is useful for Need evaluation.
     */
    static float ComputeAverageAttraction(const Agent& self, const Environment& env);
};

}  // namespace gr
