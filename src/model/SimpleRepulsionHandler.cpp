#include <glm/geometric.hpp>
#include <gramma/model/SimpleRepulsionHandler.hpp>

namespace gr {

SimpleRepulsionHandler::SimpleRepulsionHandler(float repulsionStrength) : m_RepulsionStrength(repulsionStrength) {
}

void SimpleRepulsionHandler::HandleCollisions(std::vector<Agent>& agents) const {
    for (size_t i = 0; i < agents.size(); ++i) {
        for (size_t j = i + 1; j < agents.size(); ++j) {
            Agent& a = agents[i];
            Agent& b = agents[j];
            glm::vec2 diff = a.Position - b.Position;
            float dist = glm::length(diff);
            float minDist = a.Traits.comfortRadius + b.Traits.comfortRadius;
            if (dist < minDist && dist > 0.0f) {
                glm::vec2 repulsion = glm::normalize(diff) * (minDist - dist) * m_RepulsionStrength;
                a.Velocity += repulsion;
                b.Velocity -= repulsion;
            }
        }
    }
}

}  // namespace gr
