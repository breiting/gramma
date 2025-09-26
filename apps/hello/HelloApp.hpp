#pragma once

#include <glad.h>

#include <gramma/Camera2D.hpp>
#include <gramma/IApp.hpp>
#include <gramma/Shader.hpp>

class HelloApp : public gramma::IApp {
   public:
    ~HelloApp();
    std::string Name() const override;

    bool Init(gramma::AppContext &ctx) override;
    void Update(gramma::AppContext &ctx, double /*dt*/) override;

    void Render(gramma::AppContext &ctx) override;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    gramma::Shader m_shader;
    gramma::Camera2D m_cam;
};
