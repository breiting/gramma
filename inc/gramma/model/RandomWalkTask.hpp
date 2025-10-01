#pragma once
#include <glm/glm.hpp>

#include "Agent.hpp"
#include "Task.hpp"

namespace gr {

class RandomWalkTask : public Task {
   public:
    RandomWalkTask(float d = 5.0f);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    float m_Duration;  // seconds
    float m_Elapsed = 0.0f;
};

}  // namespace gr
