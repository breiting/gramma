#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/IMovementStrategy.hpp>
#include <gramma/model/ITask.hpp>
#include <memory>

namespace gr {

class MoveTask : public ITask {
   public:
    MoveTask(const glm::vec2& target, std::unique_ptr<IMovementStrategy> strategy);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override {
        return m_Done;
    }

   private:
    glm::vec2 m_Target;
    std::unique_ptr<IMovementStrategy> m_Strategy;
    bool m_Done{false};
};

}  // namespace gr
