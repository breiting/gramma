#include "SimApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/ExerciseNeed.hpp>
#include <gramma/model/HungerNeed.hpp>
#include <gramma/model/KDTreeCollisionHandler.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/Room.hpp>
#include <gramma/model/SimAgentFactory.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>

using namespace gr;
using namespace std;

static void GenerateAgents(Environment* env) {
    if (!env) return;
    SimAgentFactory factory;
    for (int i = 0; i < 50; ++i) {
        auto agent = factory.Create(env);
        env->AddAgent(std::move(agent));
    }
}

static void GenerateFood(Environment* env) {
    if (!env) return;
    for (int i = 0; i < 5; ++i) {
        glm::vec2 pos = env->RandomPosition();
        env->AddFoodSource(std::make_shared<gr::FoodSource>(pos, 1.0, 0.1));
    }
}

std::string SimApp::Name() const {
    return "SimApp";
}

bool SimApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing SimApp..." << std::endl;

    // Room
    constexpr float border = 1.0;
    constexpr float envWidth = 50.0;
    constexpr float envHeight = 30.0;

    m_Env = std::make_unique<gr::Environment>(-envWidth / 2.0, envWidth / 2.0, -envHeight / 2.0, envHeight / 2.0);
    m_Env->SetCollisionHandler(std::make_unique<KDTreeCollisionHandler>());

    m_EnvView.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    // Setup camera
    m_Camera.SetOrthoByHeight(envHeight + border, ctx.Aspect());

    GenerateAgents(m_Env.get());
    GenerateFood(m_Env.get());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            m_SeedAgents = true;
        } else if (key == GLFW_KEY_F) {
            m_SeedFood = true;
        } else if (key == GLFW_KEY_W) {
            m_Camera.FitToEnvironment(m_Env.get(), ctx.Aspect());
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

    onWindowSize = [this](int w, int h) {
        m_Camera.FitToEnvironment(m_Env.get(), float(w) / float(h));  //
    };

    std::cout << "SimApp initialized. Starting simulation." << std::endl;
    return true;
}

void SimApp::Update(gr::AppContext& /*ctx*/, double dt) {
    TimeMeasureGuard guard("Update");

    bool drawStats = true;
    if (drawStats) {
        static float lastPrint = 0.0f;
        float currentTime = Now();
        if (currentTime - lastPrint >= 1.0f) {
            std::cout << "Time: " << currentTime << "s, dt=" << dt << std::endl;
            m_Env->Stats();
            lastPrint = currentTime;
        }
    }

    if (m_SeedAgents) {
        GenerateAgents(m_Env.get());
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
    TimeMeasureGuard guard("Render");
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_Gui->BeginFrame();

    // Dein Panel
    ImGui::Begin("Environment Stats");
    ImGui::Text("Agents: %zu", m_Env->GetAgents().size());
    ImGui::Text("FoodSources: %zu", m_Env->GetFoodSources().size());
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.5, 10.0);
    ImGui::End();

    m_Gui->EndFrame();

    m_EnvView.Draw(m_Env.get(), m_Camera);
}
