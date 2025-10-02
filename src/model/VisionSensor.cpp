#include <cmath>
#include <glm/glm.hpp>
#include <gramma/core/Environment.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <limits>
#include <nanoflann.hpp>

namespace gr {

VisionSensor::VisionSensor(float range, float fovDeg, int numRays) : m_Range(range), m_FOV(fovDeg), m_NumRays(numRays) {
}

void VisionSensor::Update(const Agent& self, const Environment& env) {
    m_Hits.clear();

    const auto& cloud = env.GetAgentCloud();
    auto& kdIndex = env.GetKDTree();

    float halfFovRad = glm::radians(m_FOV * 0.5f);
    float headingRad = glm::radians(self.GetHeading());

    for (int i = 0; i < m_NumRays; ++i) {
        float t = (m_NumRays == 1 ? 0.5f : float(i) / (m_NumRays - 1));
        float angle = headingRad - halfFovRad + t * (2.0f * halfFovRad);

        VisionHit bestHit;
        bestHit.distance = m_Range;

        // Query Agenten innerhalb Range
        {
            double query_pt[2] = {self.GetPosition().x, self.GetPosition().y};
            std::vector<nanoflann::ResultItem<size_t, double>> matches;
            nanoflann::SearchParameters params;

            kdIndex.radiusSearch(&query_pt[0], m_Range * m_Range, matches, params);

            for (auto& match : matches) {
                Agent* other = (*cloud.agents)[match.first].get();
                if (other == &self || other->GetState() == AgentState::Dead) continue;

                VisionHit hit;
                if (RayIntersectsAgent(self, *other, angle, hit)) {
                    if (hit.distance < bestHit.distance) {
                        bestHit = hit;
                        bestHit.targetAgent = other;
                    }
                }
            }
        }

        // Check Bounds
        VisionHit boundHit;
        if (RayIntersectsBounds(self.GetPosition(), angle, env, boundHit)) {
            if (boundHit.distance < bestHit.distance) {
                bestHit = boundHit;
                bestHit.targetAgent = nullptr;
            }
        }

        m_Hits.push_back(bestHit);
    }
}

bool VisionSensor::RayIntersectsAgent(const Agent& self, Agent& other, float rayAngle, VisionHit& outHit) const {
    glm::vec2 origin = self.GetPosition();
    glm::vec2 dir = {std::sin(rayAngle), std::cos(rayAngle)};
    glm::vec2 c = other.GetPosition();
    float r = other.GetTraits().bodyRadius;

    glm::vec2 oc = c - origin;
    float proj = glm::dot(oc, dir);
    if (proj <= 0) return false;

    float distSq = glm::dot(oc, oc) - proj * proj;
    if (distSq > r * r) return false;

    float thc = std::sqrt(r * r - distSq);
    float t0 = proj - thc;

    if (t0 < 0) return false;
    outHit.distance = t0;
    outHit.hitPos = origin + dir * t0;
    return true;
}

bool VisionSensor::RayIntersectsBounds(const glm::vec2& origin, float rayAngle, const Environment& env,
                                       VisionHit& outHit) const {
    glm::vec2 dir = {std::sin(rayAngle), std::cos(rayAngle)};
    float tMin = std::numeric_limits<float>::infinity();

    auto tryVertical = [&](float xPlane) {
        if (dir.x == 0.0f) return;
        float t = (xPlane - origin.x) / dir.x;
        if (t > 0) {
            float y = origin.y + dir.y * t;
            if (y >= env.YMin() && y <= env.YMax() && t < tMin) {
                tMin = t;
                outHit.hitPos = {xPlane, y};
            }
        }
    };
    auto tryHorizontal = [&](float yPlane) {
        if (dir.y == 0.0f) return;
        float t = (yPlane - origin.y) / dir.y;
        if (t > 0) {
            float x = origin.x + dir.x * t;
            if (x >= env.XMin() && x <= env.XMax() && t < tMin) {
                tMin = t;
                outHit.hitPos = {x, yPlane};
            }
        }
    };

    tryVertical(env.XMin());
    tryVertical(env.XMax());
    tryHorizontal(env.YMin());
    tryHorizontal(env.YMax());

    if (tMin < std::numeric_limits<float>::infinity() && tMin <= m_Range) {
        outHit.distance = tMin;
        return true;
    }
    return false;
}

}  // namespace gr
