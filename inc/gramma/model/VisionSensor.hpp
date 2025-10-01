#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "ISensor.hpp"

namespace gr {

struct VisionHit {
    glm::vec2 position;    // wo getroffen
    float distance;        // Distanz vom Agent
    bool isAgent = false;  // true = anderer Agent, false = Wand
};

class VisionSensor : public ISensor {
   public:
    VisionSensor(float range, float fov, int numRays);

    void Update(const Agent& agent, const Room& room) override;

    const std::vector<VisionHit>& GetHits() const {
        return m_Hits;
    }

   private:
    float m_Range;  // Sichtweite (Meter)
    float m_FOV;    // Field of View in Grad (±FOV/2 um Heading)
    int m_NumRays;  // wie viele Strahlen casten

    std::vector<VisionHit> m_Hits;

    bool IntersectRaySegment(const glm::vec2& origin, const glm::vec2& dir, const glm::vec2& a, const glm::vec2& b,
                             float& outLambda);
};

}  // namespace gr
