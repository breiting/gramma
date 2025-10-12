#include <glm/glm.hpp>
#include <gramma/model/movement/DirectMovement.hpp>
#include <gramma/model/need/SocialNeed.hpp>
#include <gramma/model/task/SocialNeedTaskBuilder.hpp>

namespace gr {

// Same matrix as in SocialNeed
static constexpr float AttractionMatrix[4][4] = {
    {+0.5f, +0.3f, -0.2f, -0.5f},  // Child
    {+0.3f, +0.8f, 0.0f, -0.2f},   // Teenager
    {-0.2f, 0.0f, +0.9f, +0.1f},   // Adult
    {-0.5f, -0.2f, +0.1f, +0.4f}   // Senior
};

std::unique_ptr<ITask> SocialNeedTaskBuilder::Build(const INeed& need, Agent& agent, const Environment& env) const {
    const auto* social = dynamic_cast<const SocialNeed*>(&need);
    if (!social) return nullptr;

    const auto& agents = env.QueryAgentsInRadius(agent.GetPosition(), agent.GetTraits().socialRadius);

    const Agent* bestTarget = nullptr;
    float bestScore = -999.0f;

    for (const Agent* other : agents) {
        if (other == &agent) continue;

        int a1 = static_cast<int>(agent.GetTraits().age);
        int a2 = static_cast<int>(other->GetTraits().age);
        float score = AttractionMatrix[a1][a2];

        if (score > bestScore) {
            bestScore = score;
            bestTarget = other;
        }
    }

    if (bestTarget) {
        return std::make_unique<SeekAgentTask>(bestTarget, std::make_unique<DirectMovement>());
    }

    return nullptr;
}

}  // namespace gr
