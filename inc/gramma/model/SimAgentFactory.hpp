#pragma once
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/AgentTraits.hpp>
#include <gramma/model/VisionSensor.hpp>

namespace gr {

class SimAgentFactory : public AgentFactory {
   public:
    std::unique_ptr<Agent> Create(const std::string& id, Environment* env) override;

   private:
    AgentTraits RandomTraits();
};

}  // namespace gr
