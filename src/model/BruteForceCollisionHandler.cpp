#include <algorithm>
#include <glm/glm.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/BruteForceCollisionHandler.hpp>

namespace gr {

void BruteForceCollisionHandler::Resolve(Environment& env) {
    auto& agents = env.GetAgents();

    // Agent vs Agent
    for (size_t i = 0; i < agents.size(); ++i) {
        for (size_t j = i + 1; j < agents.size(); ++j) {
            Agent* a = agents[i].get();
            Agent* b = agents[j].get();

            if (a->GetState() == AgentState::Dead || b->GetState() == AgentState::Dead) continue;

            glm::vec2 diff = b->GetPosition() - a->GetPosition();
            float dist = glm::length(diff);
            float minDist = a->GetTraits().comfortRadius + b->GetTraits().comfortRadius;

            if (dist < minDist && dist > 0.0001f) {
                glm::vec2 dir = diff / dist;
                glm::vec2 push = dir * (minDist - dist) * 0.5f;
                a->SetPosition(a->GetPosition() - push);
                b->SetPosition(b->GetPosition() + push);
            }
        }
    }

    // Agent vs Bounds
    for (auto& a : agents) {
        auto pos = a->GetPosition();
        float r = a->GetTraits().bodyRadius;

        pos.x = std::clamp(pos.x, env.XMin() + r, env.XMax() - r);
        pos.y = std::clamp(pos.y, env.YMin() + r, env.YMax() - r);

        a->SetPosition(pos);
    }
}

}  // namespace gr
