#pragma once

#include <glad.h>

#include <gramma/core/IApp.hpp>
#include <gramma/model/World2D.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/CircleBatch.hpp>
#include <gramma/view/LineBatch.hpp>
#include <gramma/view/Shader.hpp>

class BounceApp : public gr::IApp {
   public:
    BounceApp() = default;
    ~BounceApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double /*dt*/) override;

    void Render(gr::AppContext& ctx) override;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    gr::Shader m_shader;
    gr::Camera2D m_cam;

    gr::World2D m_world;
    gr::CircleBatch m_circles;
    gr::LineBatch m_lines;
};
