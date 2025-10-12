#include <glm/glm.hpp>
#include <gramma/model/social/SocialSensor.hpp>

namespace gr {

// Attraction values between different AgeClass groups
static constexpr float AttractionMatrix[4][4] = {
    {+0.5f, +0.3f, -0.2f, -0.5f},  // Child
    {+0.3f, +0.8f, 0.0f, -0.2f},   // Teenager
    {-0.2f, 0.0f, +0.9f, +0.1f},   // Adult
    {-0.5f, -0.2f, +0.1f, +0.4f}   // Senior
};

// ---------------------------------------------------------

std::vector<const Agent*> SocialSensor::QueryNearbyAgents(const Agent& self, const Environment& env) {
    return env.QueryAgentsInRadius(self.GetPosition(), self.GetTraits().socialRadius);
}

// ---------------------------------------------------------

const Agent* SocialSensor::FindMostAttractive(const Agent& self, const Environment& env) {
    const auto& agents = QueryNearbyAgents(self, env);

    const Agent* bestTarget = nullptr;
    float bestScore = -999.0f;

    for (const Agent* other : agents) {
        if (other == &self) continue;

        int a1 = static_cast<int>(self.GetTraits().age);
        int a2 = static_cast<int>(other->GetTraits().age);
        float score = AttractionMatrix[a1][a2];

        if (score > bestScore) {
            bestScore = score;
            bestTarget = other;
        }
    }

    return bestTarget;
}

// ---------------------------------------------------------

float SocialSensor::ComputeAverageAttraction(const Agent& self, const Environment& env) {
    const auto& agents = QueryNearbyAgents(self, env);
    if (agents.empty()) return 0.0f;

    float total = 0.0f;
    for (const Agent* other : agents) {
        if (other == &self) continue;

        int a1 = static_cast<int>(self.GetTraits().age);
        int a2 = static_cast<int>(other->GetTraits().age);
        total += AttractionMatrix[a1][a2];
    }

    // Normalize to [-1, +1]
    return glm::clamp(total / static_cast<float>(agents.size()), -1.0f, 1.0f);
}

}  // namespace gr
