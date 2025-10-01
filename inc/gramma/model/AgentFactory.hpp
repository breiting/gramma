#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/model/Room.hpp>
#include <memory>
#include <random>

namespace gr {

/**
 * Utility class to create agents with random traits and position.
 */
class AgentFactory {
   public:
    AgentFactory();

    std::unique_ptr<Agent> CreateRandomAgent(float diameter);

   private:
    std::mt19937 m_Rng;

    AgentTraits RandomTraits();
    /** \brief Generates a random position for a circle with diameter
     */
    glm::vec2 RandomPosition(float diameter);
    float RandomHeading();
};

}  // namespace gr
