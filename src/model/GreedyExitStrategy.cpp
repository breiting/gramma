#include <glm/geometric.hpp>
#include <gramma/model/GreedyExitStrategy.hpp>

namespace gr {

glm::vec2 GreedyExitStrategy::ComputeDesiredVelocity(const Agent& agent, const Exit& exit) const {
    glm::vec2 dir = exit.Position - agent.Position;
    float dist = glm::length(dir);
    if (dist > 0.0f) {
        dir = glm::normalize(dir);
        return dir * agent.Traits.speedPref;
    }
    return glm::vec2(0.0f);  // already at exit
}

}  // namespace gr
