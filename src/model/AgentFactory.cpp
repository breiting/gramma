#include <chrono>
#include <glm/glm.hpp>
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/VisionSensor.hpp>

namespace gr {

AgentFactory::AgentFactory() {
    m_Rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
}

std::unique_ptr<Agent> AgentFactory::CreateRandomAgent(float diameter) {
    AgentTraits traits = RandomTraits();
    glm::vec2 pos = RandomPosition(diameter);
    float heading = RandomHeading();

    auto agent = std::make_unique<Agent>(pos, heading, traits);

    // Add sensors
    agent->AttachSensor(std::make_unique<VisionSensor>(10.0, 120.0, 9));

    return agent;
}

AgentTraits AgentFactory::RandomTraits() {
    std::uniform_int_distribution<int> ageDist(0, 3);
    std::uniform_real_distribution<float> radiusDist(0.15f, 0.25f);
    std::uniform_real_distribution<float> comfortDist(0.26f, 0.5f);
    std::uniform_real_distribution<float> speedDist(0.8f, 4.8f);

    AgentTraits traits;
    traits.age = static_cast<AgeClass>(ageDist(m_Rng));
    traits.bodyRadius = radiusDist(m_Rng);
    traits.comfortRadius = comfortDist(m_Rng);
    traits.speedPref = speedDist(m_Rng);
    return traits;
}

glm::vec2 AgentFactory::RandomPosition(float diameter) {
    std::uniform_real_distribution<float> xDist(-diameter / 2.0, diameter / 2.0);
    std::uniform_real_distribution<float> yDist(-diameter / 2.0, diameter / 2.0);

    return glm::vec2(xDist(m_Rng), yDist(m_Rng));
}

float AgentFactory::RandomHeading() {
    std::uniform_real_distribution<float> headingDist(0.0f, 360.0f);
    return headingDist(m_Rng);
}

}  // namespace gr
