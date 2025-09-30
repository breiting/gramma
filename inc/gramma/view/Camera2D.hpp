#pragma once
#include <glm/mat4x4.hpp>

namespace gr {

/** Simple orthographic 2D camera (view-projection only). */
class Camera2D {
   public:
    Camera2D();
    void SetOrtho(float l, float r, float b, float t);
    void SetOrthoByHeight(float height, float aspect);
    void SetOrthoByWidth(float width, float aspect);
    void SetPosition(glm::vec2 pos);
    void SetZoom(float z);
    glm::mat4 ViewProj() const;

   private:
    glm::vec2 m_Position;
    float m_Left, m_Right, m_Bottom, m_Top;
    float m_Zoom;
};

}  // namespace gr
