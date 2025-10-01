#include <gramma/view/RoomView.hpp>

namespace gr {

RoomView::~RoomView() {
}

void RoomView::Init() {
    m_Shader.BuildLine();
    m_LineShape.Init();
}

void RoomView::Draw(const Room* room, const glm::mat4& vp) {
    if (!room) return;
    if (room->GetContour().empty()) return;
    m_LineShape.Set(room->GetContour());
    m_LineShape.Draw(m_Shader, vp, {0.6f, 0.6f, 0.6f, 1.0f});
}

}  // namespace gr
