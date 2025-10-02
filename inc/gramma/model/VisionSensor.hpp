#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/ISensor.hpp>
#include <vector>

namespace gr {

class Agent;
class Environment;

struct VisionHit {
    Agent* targetAgent = nullptr;  // nullptr, wenn es ein Bound war
    glm::vec2 hitPos;
    float distance = 0.0f;
};

class VisionSensor : public ISensor {
   public:
    VisionSensor(float range, float fovDeg, int numRays);

    void Update(const Agent& self, const Environment& env);
    const std::vector<VisionHit>& GetHits() const {
        return m_Hits;
    }

   private:
    float m_Range;
    float m_FOV;
    int m_NumRays;

    std::vector<VisionHit> m_Hits;

    bool RayIntersectsAgent(const Agent& self, Agent& other, float rayAngle, VisionHit& outHit) const;
    bool RayIntersectsBounds(const glm::vec2& origin, float rayAngle, const Environment& env, VisionHit& outHit) const;
};

}  // namespace gr
