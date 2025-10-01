#pragma once
#include <glm/vec2.hpp>

#include "Task.hpp"

namespace gr {

class FoodSource;

class SeekFoodTask : public Task {
   public:
    explicit SeekFoodTask(std::shared_ptr<FoodSource> target);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    std::shared_ptr<FoodSource> m_Target;
    bool m_Finished = false;
};

}  // namespace gr
