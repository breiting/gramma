#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/VisionSensor.hpp>

namespace gr {

RandomWalkTask::RandomWalkTask(float duration) : m_Duration(duration) {
}

void RandomWalkTask::Start(Agent& agent) {
    float angleDeg = static_cast<float>(rand() % 360);
    agent.Heading = angleDeg;

    float angleRad = glm::radians(angleDeg);
    agent.Velocity = glm::vec2(sin(angleRad), cos(angleRad)) * agent.Traits.speedPref;
}

void RandomWalkTask::Update(Agent& agent, float dt) {
    m_Elapsed += dt;
    if (m_Elapsed > m_Duration) return;

    agent.Position += agent.Velocity * dt;

    // Search for vision sensors
    for (auto& sensor : agent.GetSensors()) {
        auto* vision = dynamic_cast<VisionSensor*>(sensor.get());
        if (!vision) continue;

        const auto& hits = vision->GetHits();
        if (hits.empty()) continue;

        // zentraler Ray = Mitte der Liste
        const VisionHit& centerHit = hits[hits.size() / 2];

        // Wand zu nah?
        if (!centerHit.isAgent && centerHit.distance < agent.Traits.comfortRadius * 1.2f) {
            // Heading um 180° drehen
            agent.Heading = std::fmod(agent.Heading + (rand() % 180), 360.0f);

            float angleRad = glm::radians(agent.Heading);
            agent.Velocity = glm::vec2(std::sin(angleRad), std::cos(angleRad)) * agent.Traits.speedPref;

            break;  // nur einmal pro Frame reagieren
        }
    }
}

bool RandomWalkTask::IsFinished() const {
    return m_Elapsed > m_Duration;
}

}  // namespace gr
