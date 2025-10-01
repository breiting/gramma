#include "SimApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/model/Room.hpp>
#include <iostream>

#include "gramma/core/Time.hpp"
#include "gramma/model/AgentTraits.hpp"

using namespace gr;

std::string SimApp::Name() const {
    return "SimApp";
}

bool SimApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing SimApp..." << std::endl;

    // Define room and exit
    constexpr float roomWidth = 5.0;
    constexpr float roomHeight = 2.0;
    Room room;
    room.Position = glm::vec2(-roomWidth / 2.0, -roomHeight / 2.0);
    room.Size = glm::vec2(roomWidth, roomHeight);

    room.Exits.push_back({glm::vec2(roomWidth / 2.0f, 0.0f), glm::vec2(0.1f, 1.0f)});   // Right exit
    room.Exits.push_back({glm::vec2(-roomWidth / 2.0f, 0.0f), glm::vec2(0.1f, 1.0f)});  // Left exit

    // Setup camera
    m_Camera.SetOrthoByWidth(roomWidth + 10, ctx.Aspect());

    // Setup Agent
    m_AgentView.Init();
    m_Agent.Position = {0.0, 0.0};
    AgentTraits traits;
    traits.age = gr::AgeClass::Teenager;
    traits.bodyRadius = 0.25;
    traits.comfortRadius = 0.5;
    m_Agent.Traits = traits;

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_S) {
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
}

void SimApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_AgentView.Draw(m_Agent, m_Camera.ViewProj());
}
