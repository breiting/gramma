#pragma once
#include <glm/vec2.hpp>
#include <memory>

#include "Task.hpp"
#include "gramma/model/MoveTask.hpp"

namespace gr {

class FoodSource;

class SeekFoodTask : public Task {
   public:
    explicit SeekFoodTask(std::shared_ptr<FoodSource> food);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    std::shared_ptr<FoodSource> m_Food;
    std::unique_ptr<MoveTask> m_Move;
    bool m_Finished = false;
};

}  // namespace gr
