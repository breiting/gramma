#pragma once
#include <memory>

namespace gr {

class Agent;
class Room;

class ISensor {
   public:
    virtual ~ISensor() = default;

    virtual void Update(const Agent& agent, const Room& room) = 0;
};

using SensorPtr = std::unique_ptr<ISensor>;

}  // namespace gr
