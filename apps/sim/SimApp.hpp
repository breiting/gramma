#pragma once

#include <gramma/core/IApp.hpp>
#include <gramma/view/AgentView.hpp>
#include <gramma/view/Camera2D.hpp>

class SimApp : public gr::IApp {
   public:
    ~SimApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double dt) override;
    void Render(gr::AppContext& ctx) override;

   private:
    gr::Camera2D m_Camera;
    gr::AgentView m_AgentView;
    gr::Agent m_Agent;

    bool m_Restart{false};
    bool m_Quit{false};
};
