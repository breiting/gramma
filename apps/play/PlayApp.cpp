#include "PlayApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/FoodResource.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/model/SafetyNeed.hpp>
#include <gramma/model/SimAgentFactory.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <gramma/model/WalkNeed.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

#include "gramma/model/AgentSerializer.hpp"
#include "gramma/model/DirectMovement.hpp"
#include "gramma/model/MoveTask.hpp"

using namespace gr;

void PlayApp::LoadAgent(const std::string& filename) {
    AgentSerializer s;
    auto agent = s.Load(filename);
    m_Env->AddAgent(std::move(agent));
}

std::string PlayApp::Name() const {
    return "PlayApp";
}

bool PlayApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing PlayApp..." << std::endl;

    m_Env = std::make_unique<gr::Environment>(glm::vec2(0, 0));

    constexpr float border = 1.0;
    constexpr float ew = 10.0;
    constexpr float eh = 5.0;
    std::vector<glm::vec2> room = {
        {-ew / 2.0, -eh / 2.0}, {-ew / 2.0, eh / 2.0}, {ew / 2.0, eh / 2.0}, {ew / 2.0, -eh / 2.0}};
    m_Env->AddBoundary(room);

    LoadAgent("k1.json");
    LoadAgent("k2.json");

    m_EnvView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    auto height = eh + border;
    m_Camera.SetOrthoByHeight(height, ctx.Aspect());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            m_Env->Agents()[0]->AssignTask(
                std::make_unique<MoveTask>(glm::vec2(4, 0), std::make_unique<DirectMovement>()));
            m_Env->Agents()[1]->AssignTask(
                std::make_unique<MoveTask>(glm::vec2(-4, 0), std::make_unique<DirectMovement>()));
        } else if (key == GLFW_KEY_R) {
            m_Env->RemoveAllAgents();
            LoadAgent("k1.json");
            LoadAgent("k2.json");
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
            // CreateAgent(m_Env.get(), position);
        }
    };

    onWindowSize = [this](int w, int h) {
        m_Camera.FitToEnvironment(m_Env.get(), float(w) / float(h));  //
    };

    std::cout << "PlayApp initialized. Starting simulation with 100 agents." << std::endl;
    return true;
}

void PlayApp::Update(gr::AppContext& /*ctx*/, double dt) {
    m_Env->Update(static_cast<float>(dt) * m_Timescale);
    m_EnvView.SyncWithEnvironment(m_Env.get());
}

void PlayApp::Render(gr::AppContext& ctx) {
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
