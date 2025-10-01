#pragma once
#include <random>

namespace gr {

enum class AgeClass { Child, Teenager, Adult, Senior };

struct AgentTraits {
    AgeClass age;
    float bodyRadius;     // meters
    float comfortRadius;  // meters (social zone)
    float speedPref;      // meters per second
};

inline AgentTraits RandomTraits() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ageDist(0, 3);  // 0: Child, 1: Teenager, 2: Adult, 3: Senior

    AgeClass age = static_cast<AgeClass>(ageDist(gen));
    AgentTraits traits;
    traits.age = age;
    switch (age) {
        case AgeClass::Child:
            traits.bodyRadius = 0.20f;
            traits.comfortRadius = 0.40f;
            traits.speedPref = 1.2f;
            break;
        case AgeClass::Teenager:
            traits.bodyRadius = 0.25f;
            traits.comfortRadius = 0.55f;
            traits.speedPref = 1.5f;
            break;
        case AgeClass::Adult:
            traits.bodyRadius = 0.27f;
            traits.comfortRadius = 0.60f;
            traits.speedPref = 1.3f;
            break;
        case AgeClass::Senior:
            traits.bodyRadius = 0.25f;
            traits.comfortRadius = 0.60f;
            traits.speedPref = 1.0f;
            break;
    }
    return traits;
}

}  // namespace gr
