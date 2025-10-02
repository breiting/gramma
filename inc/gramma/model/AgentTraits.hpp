#pragma once

namespace gr {

enum class AgeClass { Child, Teenager, Adult, Senior };

class AgentTraits {
   public:
    virtual ~AgentTraits() = default;
    AgeClass age;
    float bodyRadius;     // meters
    float comfortRadius;  // meters (social zone)
    float speedPref;      // meters per second
};

}  // namespace gr
