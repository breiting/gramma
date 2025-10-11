#pragma once
#include <box2d/id.h>

#include <glm/vec2.hpp>

namespace gr {

enum class ResourceType { Food, Water, Home, Exit };

class IResource {
   public:
    virtual ~IResource() = default;

    virtual ResourceType GetType() const = 0;
    virtual const glm::vec2& GetPosition() const = 0;
    virtual float GetBoundingRadius() const = 0;

    /// Physics
    b2BodyId GetBody() const {
        return m_Body;
    }
    void SetBody(b2BodyId body) {
        m_Body = body;
    }

   protected:
    b2BodyId m_Body;
};

}  // namespace gr
