#include <cmath>
#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Room.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <limits>

namespace gr {

VisionSensor::VisionSensor(float range, float fov, int numRays) : m_Range(range), m_FOV(fov), m_NumRays(numRays) {
}

void VisionSensor::Update(const Agent& agent, const Room& room) {
    m_Hits.clear();

    float halfFovRad = glm::radians(m_FOV * 0.5f);
    float headingRad = glm::radians(agent.Heading);

    for (int i = 0; i < m_NumRays; ++i) {
        // berechne Winkel für diesen Ray
        float t = (m_NumRays == 1) ? 0.0f : (float(i) / (m_NumRays - 1));
        float angle = headingRad - halfFovRad + t * (2.0f * halfFovRad);

        glm::vec2 dir = glm::normalize(glm::vec2(std::sin(angle), std::cos(angle)));

        float closest = m_Range;
        glm::vec2 hitPos = agent.Position + dir * m_Range;

        // Wände checken
        for (size_t j = 0; j < room.GetContour().size(); ++j) {
            glm::vec2 a = room.GetContour()[j];
            glm::vec2 b = room.GetContour()[(j + 1) % room.GetContour().size()];

            float lambda;
            if (IntersectRaySegment(agent.Position, dir, a, b, lambda)) {
                if (lambda < closest) {
                    closest = lambda;
                    hitPos = agent.Position + dir * lambda;
                }
            }
        }

        m_Hits.push_back({hitPos, closest, false});
    }
}

bool VisionSensor::IntersectRaySegment(const glm::vec2& p, const glm::vec2& r, const glm::vec2& a, const glm::vec2& b,
                                       float& outLambda) {
    glm::vec2 s = b - a;
    float denom = r.x * s.y - r.y * s.x;
    if (std::fabs(denom) < 1e-6f) return false;  // parallel

    glm::vec2 ap = a - p;
    float lambda = (ap.x * s.y - ap.y * s.x) / denom;
    float t = (ap.x * r.y - ap.y * r.x) / denom;

    if (lambda >= 0.0f && t >= 0.0f && t <= 1.0f) {
        outLambda = lambda;
        return true;
    }
    return false;
}

}  // namespace gr
