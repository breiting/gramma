#pragma once

namespace gr {

enum class AgeClass { Child, Teenager, Adult, Senior };
enum class Sex { Male, Female };

class AgentTraits {
   public:
    virtual ~AgentTraits() = default;
    AgeClass age;
    Sex sex;
    float bodyRadius;    // meters
    float socialRadius;  // meters (social zone)
    float maxSpeed;      // meters per second
};

}  // namespace gr
