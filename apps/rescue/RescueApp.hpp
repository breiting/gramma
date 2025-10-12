#pragma once

#include <gramma/core/IApp.hpp>
#include <gramma/model/agent/SimAgentFactory.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/ui/IGuiLayer.hpp>
#include <gramma/view/AgentView.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/EnvironmentView.hpp>

class RescueApp : public gr::IApp {
   public:
    ~RescueApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double dt) override;
    void Render(gr::AppContext& ctx) override;

   private:
    void GenerateAgents(int count);
    void CreateAgent(const glm::vec2& pos);
    int Scenario1();
    int Scenario2();

   private:
    gr::Camera2D m_Camera;

    std::unique_ptr<gr::Environment> m_Env;
    gr::SimAgentFactory m_Factory;
    gr::EnvironmentView m_EnvView;

    std::unique_ptr<gr::IGuiLayer> m_Gui;

    glm::vec2 m_MousePos{0, 0};
    uint32_t m_AgentIdCounter{0};
    bool m_SeedAgents{false};
    bool m_Quit{false};
    float m_Zoom{1.0};
    float m_Timescale{1.0};
};
