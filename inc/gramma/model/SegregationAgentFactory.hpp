#pragma once
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/SatisfactionNeed.hpp>
#include <gramma/model/SegregationTraits.hpp>

namespace gr {

class SegregationAgentFactory : public AgentFactory {
   public:
    std::unique_ptr<Agent> Create(Environment* env) override;

   private:
    SegregationTraits RandomTraits();
};

}  // namespace gr
