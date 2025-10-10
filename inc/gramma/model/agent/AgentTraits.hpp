#pragma once

namespace gr {

enum class AgeClass { Child, Teenager, Adult, Senior };
enum class Sex { Male, Female };

class AgentTraits {
   public:
    virtual ~AgentTraits() = default;
    AgeClass age{AgeClass::Adult};
    Sex sex{Sex::Male};
    float mass{60};            // kg
    float bodyRadius{0.2f};    // meters
    float socialRadius{0.8f};  // meters (social zone)
    float maxSpeed{3.0f};      // meters per second
};

}  // namespace gr
