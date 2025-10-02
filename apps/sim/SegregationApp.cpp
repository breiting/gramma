#include "SegregationApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/ExerciseNeed.hpp>
#include <gramma/model/HungerNeed.hpp>
#include <gramma/model/KDTreeCollisionHandler.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/Room.hpp>
#include <gramma/model/SatisfactionNeed.hpp>
#include <gramma/model/SegregationAgentFactory.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <iostream>

using namespace gr;
using namespace std;

static void GenerateAgents(Environment* env) {
    SegregationAgentFactory factory;
    for (int i = 0; i < 50; ++i) {
        auto agent = factory.Create(env);
        env->AddAgent(std::move(agent));
    }
}

std::string SegregationApp::Name() const {
    return "SegregationApp";
}

bool SegregationApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing SegregationApp..." << std::endl;

    // Room
    constexpr float border = 1.0;
    constexpr float envWidth = 25.0;
    constexpr float envHeight = 15.0;

    m_Env = std::make_unique<gr::Environment>(-envWidth / 2.0, envWidth / 2.0, -envHeight / 2.0, envHeight / 2.0);
    m_Env->SetCollisionHandler(std::make_unique<KDTreeCollisionHandler>());

    m_EnvView.Init();

    // Setup camera
    m_Camera.SetOrthoByHeight(envHeight + border, ctx.Aspect());

    GenerateAgents(m_Env.get());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            m_SeedAgents = true;
        } else if (key == GLFW_KEY_W) {
            m_Camera.FitToEnvironment(m_Env.get(), ctx.Aspect());
        }
    };

    onWindowSize = [this](int w, int h) {
        m_Camera.FitToEnvironment(m_Env.get(), float(w) / float(h));  //
    };

    std::cout << "SegregationApp initialized. Starting simulation." << std::endl;
    return true;
}

void SegregationApp::Update(gr::AppContext& /*ctx*/, double dt) {
    TimeMeasureGuard guard("Update");

    bool drawStats = false;
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

    m_Env->Update(static_cast<float>(dt));
    m_EnvView.SyncWithEnvironment(m_Env.get());
}

void SegregationApp::Render(gr::AppContext& ctx) {
    TimeMeasureGuard guard("Render");
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_EnvView.Draw(m_Env.get(), m_Camera);
}
