#pragma once
#include "INeed.hpp"

namespace gr {

/** Hunger increases with time; if too high, the agent must seek food */
class HungerNeed : public INeed {
   public:
    HungerNeed(float rate = 0.01f);

    void Update(float dt) override;
    float Priority() const override;
    std::string Name() const override;
    void Reset() override;

   private:
    float m_Value;  // 0 = satiated, 1 = starving
    float m_Rate;   // increase per second
};

}  // namespace gr
