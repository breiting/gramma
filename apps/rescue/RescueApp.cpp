#include "RescueApp.hpp"

#include <gramma/model/GreedyExitStrategy.hpp>
#include <gramma/model/SimpleRepulsionHandler.hpp>
#include <iostream>

#include "GLFW/glfw3.h"
#include "gramma/core/AppContext.hpp"

using namespace gr;

std::string RescueApp::Name() const {
    return "RescueApp";
}

bool RescueApp::Init(gr::AppContext& ctx) {
    // Define room and exit
    constexpr float roomWidth = 50.0;
    constexpr float roomHeight = 20.0;
    Room room{glm::vec2(-roomWidth / 2.0, -roomHeight / 2.0), glm::vec2(roomWidth, roomHeight)};
    Exit exit{glm::vec2(roomWidth / 2.0, 0.0f), glm::vec2(0.1f, 1.0f)};  // Exit at right side

    // Create strategies
    auto navStrategy = std::make_unique<GreedyExitStrategy>();
    auto collisionHandler = std::make_unique<SimpleRepulsionHandler>(5.0f);

    // Create simulation
    m_Simulation = std::make_unique<Simulation>(room, exit, std::move(navStrategy), std::move(collisionHandler), 500);
    m_Simulation->Init();

    // Setup camera
    m_Camera.SetOrthoByWidth(roomWidth, ctx.Aspect());

    // Setup circle shapes
    m_CircleShapes.Init();

    // Setup shader
    try {
        m_Shader.BuildCircle();
    } catch (const std::exception& e) {
        std::cerr << "Shader build failed: " << e.what() << std::endl;
        return false;
    }

    onKeyPressed = [this](int key, int mods) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        }
    };

    return true;
}

void RescueApp::Update(gr::AppContext& ctx, double dt) {
    if (!m_Simulation->IsComplete()) {
        m_Simulation->Step(static_cast<float>(dt));
    }
}

void RescueApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_CircleShapes.Clear();

    // Add circles for agents
    const auto& agents = m_Simulation->GetAgents();
    for (const auto& agent : agents) {
        m_CircleShapes.Add(agent.Pos, agent.Radius * 2.0f);  // Green circles, diameter = 2*radius
    }

    // Render
    m_CircleShapes.Upload();
    m_CircleShapes.Draw(m_Shader, m_Camera.ViewProj(), 1.0f);
}
