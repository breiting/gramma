#include "SimApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/AgentFactory.hpp>
#include <gramma/model/ExerciseNeed.hpp>
#include <gramma/model/HungerNeed.hpp>
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
    constexpr float envWidth = 100.0;
    constexpr float envHeight = 60.0;

    m_Env = std::make_unique<gr::Environment>(-envWidth / 2.0, envWidth / 2.0, -envHeight / 2.0, envHeight / 2.0);

    // Setup camera
    m_Camera.SetOrthoByHeight(envHeight + border, ctx.Aspect());

    AgentFactory factory;
    for (int i = 0; i < 20; ++i) {
        auto agent = factory.CreateRandomAgent(envHeight);
        agent->AddNeed(std::make_unique<gr::HungerNeed>());
        agent->AddNeed(std::make_unique<gr::ExerciseNeed>());
        m_Env->AddAgent(std::move(agent));
    }

    for (int i = 0; i < 5; ++i) {
        glm::vec2 pos = {(float)(rand() % int(envWidth) - envWidth / 2.0),
                         (float)(rand() % int(envHeight) - envHeight / 2.0)};
        m_Env->AddFoodSource(std::make_shared<gr::FoodSource>(pos));
    }

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_R) {
            m_Restart = true;
        }
    };

    std::cout << "SimApp initialized. Starting simulation." << std::endl;
    return true;
}

void SimApp::Update(gr::AppContext& /*ctx*/, double dt) {
    static float lastPrint = 0.0f;
    float currentTime = Now();
    if (currentTime - lastPrint >= 1.0f) {
        std::cout << "Time: " << currentTime << "s, dt=" << dt << std::endl;
        lastPrint = currentTime;
    }
    m_Env->Update(static_cast<float>(dt));
}

void SimApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_Env->Render(m_Camera.ViewProj());
}
