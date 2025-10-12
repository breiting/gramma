#pragma once
#include <gramma/model/need/INeed.hpp>
#include <string>

namespace gr {

/**
 * @brief Models the social attraction / repulsion behavior between agents.
 * Uses the AttractionMatrix based on AgeClass similarity.
 */
class SocialNeed : public INeed {
   public:
    float Evaluate(const Agent& self, const Environment& env) const override;
    float Utility(const Agent& self, const Environment& env) const override;
    std::string Name() const override {
        return "Social";
    }
    void Reset() override {
    }
};

}  // namespace gr
