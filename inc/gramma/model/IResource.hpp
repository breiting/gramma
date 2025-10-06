#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Types.hpp>

namespace gr {

class IResource {
   public:
    virtual ~IResource() = default;

    virtual ResourceType GetType() const = 0;
    virtual const glm::vec2& GetPosition() const = 0;

    virtual float Consume(float amount) = 0;  // returns actual
    virtual void Regenerate(float dt) = 0;
    virtual bool IsDepleted() const = 0;
};

}  // namespace gr
