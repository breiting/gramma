#pragma once
#include <gramma/model/AgentTraits.hpp>

namespace gr {

enum class Group { Red, Blue };

class SegregationTraits : public AgentTraits {
   public:
    Group group;
    float minSameFraction = 0.3f;
};

}  // namespace gr
