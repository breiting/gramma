#include <chrono>
#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/Environment.hpp>

namespace gr {

AgentFactory::AgentFactory() {
    m_Rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
}

glm::vec2 AgentFactory::RandomPosition(int width, int height) {
    std::uniform_real_distribution<float> xDist(-width / 2.0f, width / 2.0f);
    std::uniform_real_distribution<float> yDist(-height / 2.0f, height / 2.0f);

    return glm::vec2(xDist(m_Rng), yDist(m_Rng));
}

float AgentFactory::RandomHeading() {
    std::uniform_real_distribution<float> headingDist(0.0f, 360.0f);
    return headingDist(m_Rng);
}

}  // namespace gr
