#pragma once
#include <gramma/view/CircleShapes.hpp>

#include "IAgentView.hpp"

namespace gr {

class FancyAgentView : public IAgentView {
   public:
    FancyAgentView();

    void Draw(const Agent& agent, const glm::mat4& vp) const override;

   private:
    CircleShapes m_Shapes;
};

}  // namespace gr
