#pragma once
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/AgentTraits.hpp>
#include <gramma/model/VisionSensor.hpp>

namespace gr {

class SimAgentFactory : public AgentFactory {
   public:
    std::unique_ptr<Agent> Create(Environment* env) override;

   private:
    AgentTraits RandomTraits();
};

}  // namespace gr
