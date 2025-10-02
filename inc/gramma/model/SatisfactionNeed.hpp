#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/INeed.hpp>

namespace gr {

class SatisfactionNeed : public INeed {
   public:
    explicit SatisfactionNeed(float weight = 1.0f) : m_Weight(weight) {
    }

    float Evaluate(const Agent& self, const Environment& env) const override;

    std::string Name() const override {
        return "Satisfaction";
    }

   private:
    float m_Weight;
};

}  // namespace gr
