#include "SimApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/ExerciseNeed.hpp>
#include <gramma/model/HungerNeed.hpp>
#include <gramma/model/KDTreeCollisionHandler.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/Room.hpp>
#include <gramma/model/VisionSensor.hpp>
#include <iostream>

using namespace gr;

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

    // Setup camera
    m_Camera.SetOrthoByHeight(envHeight + border, ctx.Aspect());

    AgentFactory factory;
    for (int i = 0; i < 20; ++i) {
        auto agent = factory.CreateRandomAgent(envHeight);
        agent->AddNeed(std::make_unique<gr::HungerNeed>(0.1));
        agent->AddNeed(std::make_unique<gr::ExerciseNeed>());
        m_Env->AddAgent(std::move(agent));
    }

    for (int i = 0; i < 5; ++i) {
        glm::vec2 pos = {(float)(rand() % int(envWidth) - envWidth / 2.0),
                         (float)(rand() % int(envHeight) - envHeight / 2.0)};
        m_Env->AddFoodSource(std::make_shared<gr::FoodSource>(pos, 1.0, 0.1));
    }

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
    static float lastPrint = 0.0f;
    float currentTime = Now();
    if (currentTime - lastPrint >= 1.0f) {
        std::cout << "Time: " << currentTime << "s, dt=" << dt << std::endl;
        m_Env->Stats();
        lastPrint = currentTime;
    }

    if (m_SeedAgents) {
        AgentFactory factory;
        for (int i = 0; i < 20; ++i) {
            auto agent = factory.CreateRandomAgent(5);
            agent->AddNeed(std::make_unique<gr::HungerNeed>(0.2));
            agent->AddNeed(std::make_unique<gr::ExerciseNeed>());
            m_Env->AddAgent(std::move(agent));
        }
        m_SeedAgents = false;
    }

    if (m_SeedFood) {
        for (int i = 0; i < 5; ++i) {
            glm::vec2 pos = {(float)(rand() % int(m_Env->GetWidth()) - m_Env->GetWidth() / 2.0),
                             (float)(rand() % int(m_Env->GetWidth()) - m_Env->GetWidth() / 2.0)};
            m_Env->AddFoodSource(std::make_shared<gr::FoodSource>(pos, 1.0, 0.1));
        }
        m_SeedFood = false;
    }

    m_Env->Update(static_cast<float>(dt));
    m_EnvView.SyncWithEnvironment(m_Env.get());
}

void SimApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_EnvView.Draw(m_Env.get(), m_Camera);
}
