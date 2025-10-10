#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/sensor/ISensor.hpp>
#include <memory>
#include <vector>

namespace gr {

struct VisionHit {
    const Agent* targetAgent = nullptr;
    glm::vec2 dir{0.0f};
    float distance = 0.0f;
    bool isAgent = true;
};

class VisionSensor : public ISensor {
   public:
    VisionSensor(float range, float fov, int rays);

    void Update(const Agent& self, const Environment& env) override;

    const std::vector<VisionHit>& GetHits() const {
        return m_Hits;
    }

   private:
    float m_Range;
    float m_Fov;
    int m_NumRays;

    std::vector<VisionHit> m_Hits;
};

using VisionSensorPtr = std::unique_ptr<VisionSensor>;

}  // namespace gr
