#pragma once
#include <glm/mat4x4.hpp>

namespace gr {

class Shader;

/** Interface for renderable objects. */
class IRenderable {
   public:
    virtual ~IRenderable() = default;

    virtual void Init() = 0;
    virtual void Clear(){};   // optional
    virtual void Upload(){};  // optional
    virtual void Draw(const Shader& shader, const glm::mat4& vp, const glm::vec4& color = glm::vec4(1.0f)) = 0;
};

}  // namespace gr
