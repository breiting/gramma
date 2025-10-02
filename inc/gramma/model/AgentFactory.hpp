#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/model/Environment.hpp>
#include <memory>
#include <random>

namespace gr {

/** Base class for agent factories */
class AgentFactory {
   public:
    AgentFactory();

    virtual ~AgentFactory() = default;

    /// Create one agent (to be overridden by specializations)
    virtual std::unique_ptr<Agent> Create(Environment* env) = 0;

   protected:
    // Utility functions for all factories
    glm::vec2 RandomPosition(int width, int height);
    float RandomHeading();

    std::mt19937 m_Rng;
};

}  // namespace gr
