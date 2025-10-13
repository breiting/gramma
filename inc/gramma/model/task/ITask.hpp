#pragma once

namespace gr {

class Agent;
class IPhysicsEngine;

class ITask {
   public:
    ITask() = default;
    virtual ~ITask() = default;

    virtual void Start(Agent& agent) = 0;
    virtual void Update(IPhysicsEngine* engine, Agent& agent, float dt) = 0;
    virtual bool IsFinished() const = 0;
};

}  // namespace gr
