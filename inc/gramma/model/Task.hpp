#pragma once
#include <memory>

namespace gr {

class Agent;

class Task {
   public:
    Task() = default;
    virtual ~Task() = default;

    virtual void Start(Agent& agent) = 0;
    virtual void Update(Agent& agent, float dt) = 0;
    virtual bool IsFinished() const = 0;
};

using TaskPtr = std::unique_ptr<Task>;

}  // namespace gr
