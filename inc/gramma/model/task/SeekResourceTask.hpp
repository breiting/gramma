#pragma once

#include <glm/vec2.hpp>
#include <gramma/model/movement/IMovementStrategy.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <gramma/model/task/MoveTask.hpp>
#include <memory>

namespace gr {

class SeekResourceTask : public ITask {
   public:
    SeekResourceTask(std::shared_ptr<IResource> resource, std::unique_ptr<IMovementStrategy> movement = nullptr,
                     float arrivalThreshold = 0.1f);

    void Start(Agent& agent) override;
    void Update(IPhysicsEngine* engine, Agent& agent, float dt) override;
    bool IsFinished() const override;

    bool HasArrived() const;
    const std::shared_ptr<IResource>& GetResource() const;

   private:
    std::shared_ptr<IResource> m_Res;
    std::unique_ptr<IMovementStrategy> m_Movement;
    std::unique_ptr<MoveTask> m_MoveTask;

    bool m_AtRes{false};
    bool m_Done{false};
    float m_Threshold;
};

}  // namespace gr
