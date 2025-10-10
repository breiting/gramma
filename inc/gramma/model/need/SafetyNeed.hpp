#pragma once
#include <gramma/model/need/INeed.hpp>
#include <string>

namespace gr {

class SafetyNeed : public INeed {
   public:
    SafetyNeed() = default;

    float Priority() const override {
        return 1.0f;
    }
    std::string Name() const override {
        return "Safety";
    }
};

}  // namespace gr
