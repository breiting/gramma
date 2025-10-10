#pragma once
#include <memory>

namespace gr {

class Agent;
class Environment;

class ISensor {
   public:
    virtual ~ISensor() = default;

    virtual void Update(const Agent& agent, const Environment& env) = 0;
};

using SensorPtr = std::unique_ptr<ISensor>;

}  // namespace gr
