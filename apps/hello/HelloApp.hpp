#pragma once

#include <glad.h>

#include <gramma/core/IApp.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/Shader.hpp>
#include <gramma/view/TriangularMesh.hpp>

class HelloApp : public gr::IApp {
   public:
    ~HelloApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double /*dt*/) override;

    void Render(gr::AppContext& ctx) override;

   private:
    gr::TriangularMesh m_Mesh;
    gr::Shader m_Shader;
    gr::Camera2D m_Cam;
};
