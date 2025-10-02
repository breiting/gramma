#pragma once

#include <gramma/core/IApp.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/view/AgentView.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/EnvironmentView.hpp>
#include <gramma/view/RoomView.hpp>

class SegregationApp : public gr::IApp {
   public:
    ~SegregationApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double dt) override;
    void Render(gr::AppContext& ctx) override;

   private:
    gr::Camera2D m_Camera;

    std::unique_ptr<gr::Environment> m_Env;
    gr::EnvironmentView m_EnvView;

    bool m_SeedAgents{false};
    bool m_Quit{false};
};
