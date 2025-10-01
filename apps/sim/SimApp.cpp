#include "SimApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/Room.hpp>
#include <iostream>

#include "gramma/core/Time.hpp"
#include "gramma/model/Agent.hpp"
#include "gramma/model/AgentTraits.hpp"

using namespace gr;

std::string SimApp::Name() const {
    return "SimApp";
}

bool SimApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing SimApp..." << std::endl;

    // Room
    constexpr float border = 0.5;
    constexpr float roomWidth = 10.0;
    constexpr float roomHeight = 6.0;
    std::vector<glm::vec2> contour = {{-roomWidth / 2.0, -roomHeight / 2.0},  //
                                      {roomWidth / 2.0, -roomHeight / 2.0},   //
                                      {roomWidth / 2.0, roomHeight / 2.0},    //
                                      {-roomWidth / 2.0, roomHeight / 2.0}};
    m_Room = std::make_unique<Room>(contour);

    // Setup camera
    m_Camera.SetOrthoByHeight(roomHeight + border, ctx.Aspect());

    // Setup Agent
    AgentTraits traits;
    traits.age = gr::AgeClass::Teenager;
    traits.bodyRadius = 0.25;
    traits.comfortRadius = 0.5;
    traits.speedPref = 1.5f;  // m/s

    m_Agent.Position = {0.0, 0.0};
    m_Agent.Traits = traits;
    m_Agent.State = gr::AgentState::Idle;

    // Init views
    m_AgentView.Init();
    m_RoomView.Init();

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_S) {
            m_Restart = true;
        } else if (key == GLFW_KEY_R) {
            m_Agent.AssignTask(std::make_unique<RandomWalkTask>(5.0f));
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
    m_Agent.Update(dt);
}

void SimApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_AgentView.Draw(m_Agent, m_Camera.ViewProj());
    m_RoomView.Draw(m_Room.get(), m_Camera.ViewProj());
}
