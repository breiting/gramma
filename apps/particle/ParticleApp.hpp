#pragma once

#include <gramma/core/IApp.hpp>
#include <gramma/model/particle/ParticleSystem.hpp>
#include <gramma/ui/IGuiLayer.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/ParticleSystemView.hpp>

class ParticleApp : public gr::IApp {
   public:
    ~ParticleApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double dt) override;
    void Render(gr::AppContext& ctx) override;

   private:
    void GenerateParticles(int count, float radius, int groups);

   private:
    gr::Camera2D m_Camera;

    std::unique_ptr<gr::ParticleSystem> m_System;
    gr::ParticleSystemView m_View;

    std::unique_ptr<gr::IGuiLayer> m_Gui;

    glm::vec2 m_MousePos{0, 0};
    bool m_Quit{false};
    float m_Zoom{1.0};
    float m_Timescale{1.0};
};
