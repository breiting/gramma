#pragma once
#include <gramma/model/INeed.hpp>
#include <string>

namespace gr {

class WalkNeed : public INeed {
   public:
    WalkNeed() = default;

    float Priority() const override {
        return 1.0f;
    }
    std::string Name() const override {
        return "Walk";
    }
};

}  // namespace gr
