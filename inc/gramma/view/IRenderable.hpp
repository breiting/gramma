#pragma once
#include <glm/mat4x4.hpp>

namespace gr {

class Shader;

/** Interface for renderable objects. */
class IRenderable {
   public:
    virtual ~IRenderable() = default;

    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual void Upload() = 0;
    virtual void Draw(const Shader& shader, const glm::mat4& vp, float alpha = 1.0f) = 0;
};

}  // namespace gr