#include <glm/gtc/matrix_transform.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/view/Camera2D.hpp>

namespace gr {

Camera2D::Camera2D() : m_Left(-1.6f), m_Right(1.6f), m_Bottom(-0.9f), m_Top(0.9f), m_Zoom(1.0f) {
}

void Camera2D::SetOrtho(float l, float r, float b, float t) {
    m_Left = l;
    m_Right = r;
    m_Bottom = b;
    m_Top = t;
}

void Camera2D::SetOrthoByHeight(float height, float aspect) {
    float halfH = height * 0.5f;
    float halfW = halfH * aspect;
    m_Left = -halfW;
    m_Right = +halfW;
    m_Bottom = -halfH;
    m_Top = +halfH;
}

void Camera2D::SetOrthoByWidth(float width, float aspect) {
    float halfW = width * 0.5f;
    float halfH = halfW / aspect;
    m_Left = -halfW;
    m_Right = +halfW;
    m_Bottom = -halfH;
    m_Top = +halfH;
}

void Camera2D::SetPosition(glm::vec2 pos) {
    m_Position = pos;
}
void Camera2D::SetZoom(float z) {
    m_Zoom = z;
}

void Camera2D::FitToEnvironment(Environment *env, float aspect) {
    if (!env) return;
    m_Zoom = 1.0;
    float width = 10;  // TODO:
    float height = 10 * aspect;
    float worldAspect = width / height;

    if (worldAspect > aspect) {
        SetOrthoByWidth(width * 1.1f, aspect);  // 10% Margin
    } else {
        float targetHeight = height * 1.1f;
        float targetWidth = targetHeight * aspect;
        SetOrthoByWidth(targetWidth, aspect);
    }
}

glm::mat4 Camera2D::ViewProj() const {
    const float cx = m_Position.x, cy = m_Position.y;
    glm::mat4 V = glm::translate(glm::mat4(1.f), glm::vec3(-cx, -cy, 0.f));
    float zl = (m_Right - m_Left) * 0.5f / m_Zoom;
    float zt = (m_Top - m_Bottom) * 0.5f / m_Zoom;
    glm::mat4 P = glm::ortho(-zl, zl, -zt, zt, -1.f, 1.f);
    return P * V;
}

}  // namespace gr
