#include <glm/glm.hpp>
#include <gramma/model/ExerciseNeed.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/VisionSensor.hpp>

namespace gr {

RandomWalkTask::RandomWalkTask(float duration) : m_Duration(duration), m_Elapsed(0.0f) {
}

void RandomWalkTask::Start(Agent& agent) {
    float angleDeg = static_cast<float>(rand() % 360);
    agent.SetHeading(angleDeg);
    agent.SetSpeed(agent.GetTraits()->maxSpeed);
}

void RandomWalkTask::Update(Agent& agent, float dt) {
    m_Elapsed += dt;
    if (m_Elapsed > m_Duration) return;

    // Bewegung
    float angleRad = glm::radians(agent.GetHeading());
    glm::vec2 velocity = glm::vec2(std::sin(angleRad), std::cos(angleRad)) * agent.GetSpeed();
    agent.SetPosition(agent.GetPosition() + velocity * dt);

    // ExerciseNeed reduzieren
    for (auto& n : agent.GetNeeds()) {
        if (n->Name() == "Exercise") {
            auto* ex = dynamic_cast<ExerciseNeed*>(n.get());
            if (ex) ex->Decrease(dt);
        }
    }

    // Vision-Sensor prüfen
    for (auto& sensor : agent.GetSensors()) {
        auto* vision = dynamic_cast<VisionSensor*>(sensor.get());
        if (!vision) continue;

        const auto& hits = vision->GetHits();
        if (hits.empty()) continue;

        // Zentraler Ray
        const VisionHit& centerHit = hits[hits.size() / 2];

        // Wand zu nah?
        if (!centerHit.targetAgent && centerHit.distance < agent.GetTraits()->socialRadius * 1.2f) {
            // Heading zufällig in neuen Bereich drehen
            float turnAngle = static_cast<float>((rand() % 180) - 90);  // -90° bis +90°
            agent.SetHeading(std::fmod(agent.GetHeading() + turnAngle + 360.0f, 360.0f));

            // Geschwindigkeit beibehalten
            agent.SetSpeed(agent.GetTraits()->maxSpeed);

            break;  // nur einmal pro Frame reagieren
        }
    }
}

bool RandomWalkTask::IsFinished() const {
    return m_Elapsed > m_Duration;
}

}  // namespace gr
