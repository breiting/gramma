#pragma once
#include <memory>

namespace gr {
class ITask;
class Agent;
class Environment;
class INeed;

class TaskFactory {
   public:
    std::unique_ptr<ITask> MakeFor(const INeed& need, Agent& agent, const Environment& env);
};

}  // namespace gr
