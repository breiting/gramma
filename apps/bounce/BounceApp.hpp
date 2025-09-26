#pragma once

#include <glad.h>

#include <gramma/Camera2D.hpp>
#include <gramma/CircleBatch.hpp>
#include <gramma/IApp.hpp>
#include <gramma/LineBatch.hpp>
#include <gramma/Shader.hpp>
#include <gramma/World2D.hpp>

class BounceApp : public gramma::IApp {
   public:
    BounceApp() = default;
    ~BounceApp() = default;
    std::string Name() const override;

    bool Init(gramma::AppContext& ctx) override;
    void Update(gramma::AppContext& ctx, double /*dt*/) override;

    void Render(gramma::AppContext& ctx) override;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    gramma::Shader m_shader;
    gramma::Camera2D m_cam;

    gramma::World2D m_world;
    gramma::CircleBatch m_circles;
    gramma::LineBatch m_lines;
};
