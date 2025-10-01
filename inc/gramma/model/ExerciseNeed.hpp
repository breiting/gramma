#pragma once
#include "INeed.hpp"

namespace gr {

/**
 * ExerciseNeed: grows when the agent is idle or walking too little,
 * decreases when performing RandomWalk (movement).
 */
class ExerciseNeed : public INeed {
   public:
    ExerciseNeed(float rate = 0.005f);

    void Update(float dt) override;
    float Priority() const override;
    std::string Name() const override;
    void Reset() override;

    void Increase(float dt);  // used when agent is inactive
    void Decrease(float dt);  // used when agent is exercising

   private:
    float m_Value;  // 0 = satisfied, 1 = urgent
    float m_Rate;   // growth rate
};

}  // namespace gr
