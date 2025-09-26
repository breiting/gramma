#pragma once

#include <glad.h>

#include <gramma/Camera2D.hpp>
#include <gramma/IApp.hpp>
#include <gramma/Shader.hpp>

class HelloApp : public gramma::IApp {
   public:
    ~HelloApp();
    const char* Name() const override;

    bool Init() override;
    void Update(double /*dt*/) override;

    void Render() override;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    gramma::Shader m_shader;
    gramma::Camera2D m_cam;
};
