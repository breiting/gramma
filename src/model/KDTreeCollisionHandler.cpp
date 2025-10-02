#include <glm/glm.hpp>
#include <gramma/core/Environment.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/KDTreeCollisionHandler.hpp>

namespace gr {

void KDTreeCollisionHandler::Resolve(Environment& env) {
    auto& kdIndex = env.GetKDTree();
    const auto& cloud = env.GetAgentCloud();
    auto& agents = env.GetAgents();

    // Agent vs Agent
    for (size_t i = 0; i < agents.size(); ++i) {
        Agent* a = agents[i].get();
        if (a->GetState() == AgentState::Dead) continue;

        double query_pt[2] = {a->GetPosition().x, a->GetPosition().y};
        std::vector<nanoflann::ResultItem<size_t, double>> retMatches;
        nanoflann::SearchParameters params;

        float radius = a->GetTraits().bodyRadius * 2.0f;
        kdIndex.radiusSearch(query_pt, radius * radius, retMatches, params);

        for (auto& match : retMatches) {
            Agent* b = (*cloud.agents)[match.first].get();
            if (a == b || b->GetState() == AgentState::Dead) continue;

            glm::vec2 diff = b->GetPosition() - a->GetPosition();
            float dist = glm::length(diff);
            float minDist = a->GetTraits().bodyRadius + b->GetTraits().bodyRadius;

            if (dist < minDist && dist > 0.0001f) {
                glm::vec2 dir = diff / dist;
                glm::vec2 push = dir * (minDist - dist) * 0.5f;
                a->SetPosition(a->GetPosition() - push);
                b->SetPosition(b->GetPosition() + push);
            }
        }
    }

    // Agent vs Bounds (O(n))
    for (auto& a : agents) {
        auto pos = a->GetPosition();
        float r = a->GetTraits().bodyRadius;

        pos.x = std::clamp(pos.x, env.XMin() + r, env.XMax() - r);
        pos.y = std::clamp(pos.y, env.YMin() + r, env.YMax() - r);

        a->SetPosition(pos);
    }
}

}  // namespace gr
