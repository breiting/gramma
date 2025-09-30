#pragma once

#include <gramma/core/IApp.hpp>
#include <gramma/model/Simulation.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/CircleShapes.hpp>
#include <gramma/view/Shader.hpp>
#include <memory>

class RescueApp : public gr::IApp {
   public:
    ~RescueApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double dt) override;
    void Render(gr::AppContext& ctx) override;

   private:
    std::unique_ptr<gr::Simulation> m_Simulation;
    gr::Camera2D m_Camera;
    gr::CircleShapes m_CircleShapes;
    gr::Shader m_Shader;
    bool m_Quit{false};
};
