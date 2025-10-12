#include <glm/glm.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/need/SocialNeed.hpp>

namespace gr {

// Attraction values between different AgeClass groups
static constexpr float AttractionMatrix[4][4] = {
    {+0.5f, +0.3f, -0.2f, -0.5f},  // Child
    {+0.3f, +0.8f, 0.0f, -0.2f},   // Teenager
    {-0.2f, 0.0f, +0.9f, +0.1f},   // Adult
    {-0.5f, -0.2f, +0.1f, +0.4f}   // Senior
};

float SocialNeed::Evaluate(const Agent& self, const Environment& env) const {
    auto nearby = env.QueryAgentsInRadius(self.GetPosition(), self.GetTraits().socialRadius);
    if (nearby.empty()) return 0.0f;

    float sum = 0.0f;
    for (const auto* other : nearby) {
        int a1 = static_cast<int>(self.GetTraits().age);
        int a2 = static_cast<int>(other->GetTraits().age);
        sum += AttractionMatrix[a1][a2];
    }

    // Average attraction in the area
    return sum / static_cast<float>(nearby.size());
}

float SocialNeed::Utility(const Agent& self, const Environment& env) const {
    float score = Evaluate(self, env);
    // Normalize to [0,1]
    return glm::clamp((score + 1.0f) * 0.5f, 0.0f, 1.0f);
}

}  // namespace gr
