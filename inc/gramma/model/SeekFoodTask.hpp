#pragma once
#include <glm/vec2.hpp>

#include "Task.hpp"

namespace gr {

class FoodSource;

class SeekFoodTask : public Task {
   public:
    explicit SeekFoodTask(const FoodSource* target);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    const FoodSource* m_Target;
    bool m_Finished = false;
};

}  // namespace gr
