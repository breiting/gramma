#pragma once
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <memory>
#include <random>

namespace gr {

/** Base class for agent factories */
class AgentFactory {
   public:
    AgentFactory();

    virtual ~AgentFactory() = default;

    /// Create one agent (to be overridden by specializations)
    virtual std::unique_ptr<Agent> Create(const std::string& id, const glm::vec2& pos) = 0;

   protected:
    float RandomHeading();

    std::mt19937 m_Rng;
};

}  // namespace gr
