#pragma once
#include <gramma/model/agent/AgentFactory.hpp>
#include <gramma/model/agent/AgentTraits.hpp>
#include <gramma/model/sensor/VisionSensor.hpp>

namespace gr {

class SimAgentFactory : public AgentFactory {
   public:
    std::unique_ptr<Agent> Create(const std::string& id, const glm::vec2& pos) override;

   private:
    AgentTraits RandomTraits();
};

}  // namespace gr
