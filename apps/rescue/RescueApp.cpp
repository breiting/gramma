#include "RescueApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/agent/SimAgentFactory.hpp>
#include <gramma/model/need/EnergyNeed.hpp>
#include <gramma/model/need/SafetyNeed.hpp>
#include <gramma/model/need/WalkNeed.hpp>
#include <gramma/model/resource/FoodResource.hpp>
#include <gramma/model/resource/Home.hpp>
#include <gramma/model/sensor/VisionSensor.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>

using namespace gr;

void RescueApp::GenerateAgents(int count) {
    for (int i = 0; i < count; ++i) {
        auto agent = m_Factory.CreateAgent("Agent" + std::to_string(m_AgentIdCounter++), m_Env->RandomPosition());

        m_Env->AddAgent(std::move(agent));
    }
}

void RescueApp::CreateAgent(const glm::vec2& pos) {
    auto agent = m_Factory.CreateAgent("Agent" + std::to_string(m_AgentIdCounter++), pos);
    agent->AddNeed(std::make_unique<SafetyNeed>());

    m_Env->AddAgent(std::move(agent));
}

std::string RescueApp::Name() const {
    return "RescueApp";
}

int RescueApp::Scenario1() {
    constexpr float border = 1.0;
    constexpr float ew = 50.0;
    constexpr float eh = 30.0;
    std::vector<glm::vec2> room = {
        {-ew / 2.0, -eh / 2.0}, {-ew / 2.0, eh / 2.0}, {ew / 2.0, eh / 2.0}, {ew / 2.0, -eh / 2.0}};
    m_Env->AddBoundary(room);

    std::vector<glm::vec2> inner = {{-5, -5}, {5, -5}, {5, 5}, {-5, 5}};
    m_Env->AddObstacle(inner);

    // Add one exit
    m_Env->AddResource(std::make_shared<Exit>(glm::vec2(ew / 2.0, 0)));
    GenerateAgents(1);
    return eh + border;
}

int RescueApp::Scenario2() {
    constexpr float border = 1.0;
    constexpr float ew = 50.0;
    constexpr float eh = 30.0;
    std::vector<glm::vec2> room = {
        {-ew / 2.0, -eh / 2.0}, {-ew / 2.0, eh / 2.0}, {ew / 2.0, eh / 2.0}, {ew / 2.0, -eh / 2.0}};
    m_Env->AddBoundary(room);
    // Add two exits
    m_Env->AddResource(std::make_shared<Exit>(glm::vec2(ew / 2.0, 0)));
    m_Env->AddResource(std::make_shared<Exit>(glm::vec2(-ew / 2.0, 0)));
    GenerateAgents(500);
    return eh + border;
}

bool RescueApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing RescueApp..." << std::endl;

    m_Factory.InitTaskFactory();

    m_Env = std::make_unique<gr::Environment>();

    // auto height = Scenario1();
    auto height = Scenario2();

    m_EnvView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    m_Camera.SetOrthoByHeight(height, ctx.Aspect());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            m_SeedAgents = true;
        } else if (key == GLFW_KEY_N) {
            for (auto& a : m_Env->GetAgents()) {
                a->AddNeed(std::make_unique<SafetyNeed>());
            }

        } else if (key == GLFW_KEY_W) {
            m_Camera.FitToEnvironment(m_Env.get(), ctx.Aspect());
        }
    };

    onScroll = [this](double /*xoffs*/, double yoffs) {
        m_Zoom += yoffs * 0.02f;
        m_Camera.SetZoom(m_Zoom);
    };

    onMouseMove = [this](double x, double y) {
        m_MousePos = glm::vec2(x, y);  //
    };

    onMouseButton = [this, &ctx](int button, int action, int /*mod*/) {
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
            auto position = m_Camera.ScreenToWorld(m_MousePos, ctx.GetWidth(), ctx.GetHeight());
            CreateAgent(position);
        }
    };

    onWindowSize = [this](int w, int h) {
        m_Camera.FitToEnvironment(m_Env.get(), float(w) / float(h));  //
    };

    std::cout << "RescueApp initialized. Starting simulation with 100 agents." << std::endl;
    return true;
}

void RescueApp::Update(gr::AppContext& /*ctx*/, double dt) {
    if (m_SeedAgents) {
        GenerateAgents(100);
        m_SeedAgents = false;
    }

    m_Env->Update(static_cast<float>(dt) * m_Timescale);
    m_EnvView.SyncWithEnvironment(m_Env.get());
}

void RescueApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_Gui->BeginFrame();

    ImGui::Begin("Environment Stats");
    ImGui::Text("Agents: %zu", m_Env->GetAgents().size());
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.5, 10.0);
    ImGui::End();

    m_Gui->EndFrame();

    m_EnvView.Draw(m_Env.get(), m_Camera);
}
