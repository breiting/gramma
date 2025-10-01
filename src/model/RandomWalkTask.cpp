#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/VisionSensor.hpp>

namespace gr {

RandomWalkTask::RandomWalkTask(float duration) : m_Duration(duration), m_Elapsed(0.0f) {
}

void RandomWalkTask::Start(Agent& agent) {
    float angleDeg = static_cast<float>(rand() % 360);
    agent.SetHeading(angleDeg);
    agent.SetDesiredSpeed(agent.GetTraits().speedPref);
}

void RandomWalkTask::Update(Agent& agent, float dt) {
    m_Elapsed += dt;
    if (m_Elapsed > m_Duration) return;

    // Bewegung erfolgt über Agent::UpdateKinematics in Agent::Update()

    // Vision-Sensor prüfen
    for (auto& sensor : agent.GetSensors()) {
        auto* vision = dynamic_cast<VisionSensor*>(sensor.get());
        if (!vision) continue;

        const auto& hits = vision->GetHits();
        if (hits.empty()) continue;

        // Zentraler Ray
        const VisionHit& centerHit = hits[hits.size() / 2];

        // Wand zu nah?
        if (!centerHit.isAgent && centerHit.distance < agent.GetTraits().comfortRadius * 1.2f) {
            // Heading zufällig in neuen Bereich drehen
            float turnAngle = static_cast<float>((rand() % 180) - 90);  // -90° bis +90°
            agent.SetHeading(std::fmod(agent.GetHeading() + turnAngle + 360.0f, 360.0f));

            // Geschwindigkeit beibehalten
            agent.SetDesiredSpeed(agent.GetTraits().speedPref);

            break;  // nur einmal pro Frame reagieren
        }
    }
}

bool RandomWalkTask::IsFinished() const {
    return m_Elapsed > m_Duration;
}

}  // namespace gr
