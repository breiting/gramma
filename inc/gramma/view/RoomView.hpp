#pragma once
#include <gramma/model/Room.hpp>
#include <gramma/view/Shader.hpp>

#include "gramma/view/LineShape.hpp"

namespace gr {

class RoomView {
   public:
    RoomView() = default;
    ~RoomView();

    void Init();
    void Draw(const Room* room, const glm::mat4& vp);

   private:
    Shader m_Shader;
    LineShape m_LineShape;
};

}  // namespace gr
