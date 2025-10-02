#include <cmath>
#include <gramma/model/VisionSensor.hpp>
#include <nanoflann.hpp>

#include "glm/trigonometric.hpp"

namespace gr {

VisionSensor::VisionSensor(float range, float fov, int rays) : m_Range(range), m_Fov(fov), m_NumRays(rays) {
    m_Hits.resize(rays);
}

void VisionSensor::Update(const Agent& self, const Environment& env) {
    // Reset Hits
    for (auto& hit : m_Hits) {
        hit.targetAgent = nullptr;
        hit.distance = m_Range;
        hit.isAgent = true;
    }

    // 1. KD-Tree Query: alle Nachbarn im Umkreis holen
    double query_pt[2] = {self.GetPosition().x, self.GetPosition().y};
    std::vector<nanoflann::ResultItem<size_t, double>> neighbors;
    nanoflann::SearchParameters params;
    env.GetKDTree().radiusSearch(query_pt, m_Range * m_Range, neighbors, params);

    // 2. Treffer auf Rays verteilen
    glm::vec2 selfPos = self.GetPosition();
    float headingRad = glm::radians(self.GetHeading());

    for (auto& n : neighbors) {
        const Agent* other = env.GetAgent(n.first);
        if (!other || other == &self || other->GetState() == AgentState::Dead) continue;

        glm::vec2 dir = other->GetPosition() - selfPos;
        float dist = glm::length(dir);
        if (dist < 1e-6f) continue;

        dir /= dist;

        // Winkel berechnen relativ zur Heading-Achse
        float angle = atan2(dir.x, dir.y) - headingRad;  // (0 = nach vorne)
        angle = glm::degrees(angle);

        // Normalisieren auf [-180,180]
        if (angle > 180.0f) angle -= 360.0f;
        if (angle < -180.0f) angle += 360.0f;

        // Fällt der Nachbar ins Sichtfeld?
        if (fabs(angle) > m_Fov * 0.5f) continue;

        // Welcher Ray ist am nächsten?
        int rayIdx = static_cast<int>((angle + m_Fov * 0.5f) / m_Fov * m_NumRays);
        rayIdx = glm::clamp(rayIdx, 0, m_NumRays - 1);

        // Wenn dieser Ray noch keinen Hit oder dieser näher ist → übernehmen
        if (dist < m_Hits[rayIdx].distance) {
            m_Hits[rayIdx].targetAgent = other;
            m_Hits[rayIdx].dir = dir;
            m_Hits[rayIdx].distance = dist;
            m_Hits[rayIdx].isAgent = true;
        }
    }
}

}  // namespace gr
