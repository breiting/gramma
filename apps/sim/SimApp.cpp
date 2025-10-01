#include "SimApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/Room.hpp>
#include <iostream>

#include "gramma/core/Time.hpp"
#include "gramma/model/Agent.hpp"
#include "gramma/model/AgentFactory.hpp"
#include "gramma/model/VisionSensor.hpp"

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
    AgentFactory factory;
    for (int i = 0; i < 2; ++i) {
        auto agent = factory.CreateRandomAgent(1.0);
        agent->AttachSensor(std::make_unique<VisionSensor>(1, 30, 1));
        m_Agents.push_back(std::move(agent));

        auto view = std::make_unique<AgentView>();
        view->Init();
        m_AgentViews.push_back(std::move(view));
    }

    m_RoomView.Init();

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_S) {
            m_Restart = true;
        } else if (key == GLFW_KEY_R) {
            for (auto& a : m_Agents) {
                a->AssignTask(std::make_unique<RandomWalkTask>(50.0f));
            }
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
    for (auto& a : m_Agents) {
        a->Update(dt, m_Room.get());
    }
}

void SimApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    for (size_t i = 0; i < m_Agents.size(); i++) {
        m_AgentViews[i]->Draw(m_Agents[i].get(), m_Camera.ViewProj());
    }
    m_RoomView.Draw(m_Room.get(), m_Camera.ViewProj());
}
