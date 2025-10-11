#pragma once
#include <glm/vec2.hpp>

namespace gr {

class IConsumable {
   public:
    virtual ~IConsumable() = default;

    virtual float Consume(float amount) = 0;
    virtual void Regenerate(float dt) = 0;
    virtual bool IsDepleted() const = 0;
};

}  // namespace gr
