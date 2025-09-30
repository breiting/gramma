#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/model/ICollisionHandler.hpp>

namespace gr {

/** Simple collision handler using repulsion forces. */
class SimpleRepulsionHandler : public ICollisionHandler {
   public:
    SimpleRepulsionHandler(float repulsionStrength = 1.0f);
    void HandleCollisions(std::vector<Agent>& agents) const override;

   private:
    float m_RepulsionStrength;
};

}  // namespace gr