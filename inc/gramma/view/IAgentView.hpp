#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <gramma/model/Agent.hpp>

namespace gr {

struct IAgentView {
    virtual ~IAgentView() = default;
    virtual void Init() = 0;
    virtual void Draw(const Agent& agent, const glm::mat4& vp) = 0;
};

}  // namespace gr
