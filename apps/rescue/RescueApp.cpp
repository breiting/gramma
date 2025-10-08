#include "RescueApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/FoodResource.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/model/SimAgentFactory.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <gramma/model/WalkNeed.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>

using namespace gr;

void RescueApp::GenerateAgents(Environment* env, int count) {
    if (!env) return;
    SimAgentFactory factory;
    for (int i = 0; i < count; ++i) {
        auto agent = factory.Create("Agent" + std::to_string(m_AgentIdCounter++), env->RandomPosition());
        agent->AddNeed(std::make_unique<WalkNeed>());
        Home* home = env->GetNextFreeHome();
        if (home) {
            agent->SetHome(home);
            home->Enter(agent.get());
        }

        env->AddAgent(std::move(agent));
    }
}

void RescueApp::CreateAgent(Environment* env, const glm::vec2& pos) {
    if (!env) return;
    SimAgentFactory factory;
    auto agent = factory.Create("Agent" + std::to_string(m_AgentIdCounter++), pos);
    // agent->AddNeed(std::make_unique<WalkNeed>());

    env->AddAgent(std::move(agent));
}

std::string RescueApp::Name() const {
    return "RescueApp";
}

bool RescueApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing RescueApp..." << std::endl;

    constexpr float border = 1.0;
    constexpr float ew = 50.0;
    constexpr float eh = 30.0;

    m_Env = std::make_unique<gr::Environment>(glm::vec2(0, 0));

    // L-förmiger Raum mit einer Aussparung oben rechts
    std::vector<glm::vec2> room = {
        {-ew / 2.0f, -eh / 2.0f},  // unten links
        {ew / 2.0f, -eh / 2.0f},   // unten rechts
        {ew / 2.0f, eh / 4.0f},    // Einschnitt unten an der rechten Seite
        {ew / 4.0f, eh / 4.0f},    // Ecke nach innen
        {ew / 4.0f, eh / 2.0f},    // hoch zur oberen Kante
        {-ew / 2.0f, eh / 2.0f}    // oben links
    };
    // std::vector<glm::vec2> room = {
    //     {-ew / 2.0, -eh / 2.0}, {ew / 2.0, -eh / 2.0}, {ew / 2.0, eh / 2.0}, {-ew / 2.0, eh / 2.0}};
    m_Env->AddBoundary(room);

    // Innen (z.B. ein quadratisches Loch in der Mitte)
    std::vector<glm::vec2> inner = {{-10, -10}, {10, -10}, {10, 10}, {-10, 10}};
    m_Env->AddObstacle(inner);

    m_EnvView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    // Setup camera
    m_Camera.SetOrthoByHeight(eh + border, ctx.Aspect());

    // GenerateAgents(m_Env.get(), 50);

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            m_SeedAgents = true;
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
            CreateAgent(m_Env.get(), position);
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
        GenerateAgents(m_Env.get(), 50);
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
    ImGui::Text("Agents: %zu", m_Env->Agents().size());
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.5, 10.0);
    ImGui::End();

    m_Gui->EndFrame();

    m_EnvView.Draw(m_Env.get(), m_Camera);
}
