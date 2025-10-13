#include "SimApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/agent/SimAgentFactory.hpp>
#include <gramma/model/need/EnergyNeed.hpp>
#include <gramma/model/need/SocialNeed.hpp>
#include <gramma/model/resource/FoodResource.hpp>
#include <gramma/model/resource/Home.hpp>
#include <gramma/model/sensor/VisionSensor.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>

using namespace gr;
using namespace std;

void SimApp::GenerateAgents() {
    SimAgentFactory factory;
    for (int i = 0; i < 5000; ++i) {
        auto agent = factory.CreateAgent("Agent" + std::to_string(m_AgentIdCounter++), m_Env->RandomPosition());
        m_Env->AddAgent(std::move(agent));
    }
}

static void GenerateFood(Environment* env) {
    if (!env) return;
    for (int i = 0; i < 1; ++i) {
        glm::vec2 pos = env->RandomPosition();
        env->AddResource(std::make_shared<gr::FoodResource>(pos, 1.0, 0.1));
    }
}

static void GenerateHome(Environment* env) {
    if (!env) return;
    for (int i = 0; i < 5; ++i) {
        glm::vec2 pos = env->RandomPosition();
        env->AddResource(std::make_unique<gr::Home>(pos, 4));
    }
}

std::string SimApp::Name() const {
    return "SimApp";
}

bool SimApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing SimApp..." << std::endl;

    m_Factory.InitTaskFactory();

    // Room
    constexpr float border = 1.0;
    constexpr float ew = 250.0;
    constexpr float eh = 150.0;

    m_Env = std::make_unique<gr::Environment>();

    std::vector<glm::vec2> room = {
        {-ew / 2.0, -eh / 2.0}, {-ew / 2.0, eh / 2.0}, {ew / 2.0, eh / 2.0}, {ew / 2.0, -eh / 2.0}};
    m_Env->AddBoundary(room);

    m_EnvView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    // Setup camera
    m_Camera.SetOrthoByHeight(eh + border, ctx.Aspect());

    // GenerateHome(m_Env.get());
    GenerateAgents();
    // GenerateFood(m_Env.get());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            m_SeedAgents = true;
        } else if (key == GLFW_KEY_F) {
            m_SeedFood = true;
        } else if (key == GLFW_KEY_W) {
            m_Camera.FitToEnvironment(m_Env.get(), ctx.Aspect());
        } else if (key == GLFW_KEY_S) {
            for (auto& a : m_Env->GetAgents()) {
                a->AddNeed(std::make_unique<SocialNeed>());
            }
        } else if (key == GLFW_KEY_PAGE_UP) {
            m_Zoom += .1f;
            m_Camera.SetZoom(m_Zoom);
        } else if (key == GLFW_KEY_PAGE_DOWN) {
            m_Zoom -= 0.1f;
            m_Camera.SetZoom(m_Zoom);
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
            auto res = m_Env->QueryAgentsInRadius(position, 10.0);
            for (auto& a : res) {
                a->Print();
            }
        }
    };

    onWindowSize = [this](int w, int h) {
        m_Camera.FitToEnvironment(m_Env.get(), float(w) / float(h));  //
    };

    std::cout << "SimApp initialized. Starting simulation." << std::endl;
    return true;
}

void SimApp::Update(gr::AppContext& /*ctx*/, double dt) {
    // TimeMeasureGuard guard("Update");

    if (m_SeedAgents) {
        GenerateAgents();
        m_SeedAgents = false;
    }

    if (m_SeedFood) {
        GenerateFood(m_Env.get());
        m_SeedFood = false;
    }

    m_Env->Update(static_cast<float>(dt) * m_Timescale);
    m_EnvView.SyncWithEnvironment(m_Env.get());
}

void SimApp::Render(gr::AppContext& ctx) {
    // TimeMeasureGuard guard("Render");
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }
    m_EnvView.Draw(m_Env.get(), m_Camera);

    m_Gui->BeginFrame();

    // Dein Panel
    ImGui::Begin("Environment Stats");
    ImGui::Text("Agents: %zu", m_Env->GetAgents().size());
    ImGui::Text("FoodSources: %zu", m_Env->GetResources().size());
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.5, 10.0);
    ImGui::End();

    m_Gui->EndFrame();
}
