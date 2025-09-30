#include "RescueApp.hpp"

#include <GLFW/glfw3.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/model/GreedyExitStrategy.hpp>
#include <gramma/model/SimpleRepulsionHandler.hpp>
#include <iostream>

using namespace gr;

std::string RescueApp::Name() const {
    return "RescueApp";
}

bool RescueApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing RescueApp..." << std::endl;

    // Define room and exit
    constexpr float roomWidth = 50.0;
    constexpr float roomHeight = 20.0;
    Room room{glm::vec2(-roomWidth / 2.0, -roomHeight / 2.0), glm::vec2(roomWidth, roomHeight)};
    room.exits.push_back({glm::vec2(roomWidth / 2.0f, 0.0f), glm::vec2(0.1f, 1.0f)});   // Right exit
    room.exits.push_back({glm::vec2(-roomWidth / 2.0f, 0.0f), glm::vec2(0.1f, 1.0f)});  // Left exit

    // Create strategies
    auto navStrategy = std::make_unique<GreedyExitStrategy>();
    auto collisionHandler = std::make_unique<SimpleRepulsionHandler>(2.0f);

    // Create simulation
    m_Simulation = std::make_unique<Simulation>(room, std::move(navStrategy), std::move(collisionHandler), 500);
    m_Simulation->Init();

    // Setup camera
    m_Camera.SetOrthoByWidth(roomWidth, ctx.Aspect());

    // Setup shapes
    m_BodyShapes.Init();
    m_ComfortShapes.Init();

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
        } else if (key == GLFW_KEY_S) {
            m_Restart = true;
        }
    };

    std::cout << "RescueApp initialized. Starting simulation with 100 agents." << std::endl;
    return true;
}

void RescueApp::Update(gr::AppContext& ctx, double dt) {
    if (m_Simulation->IsComplete()) {
        std::cout << "Simulation complete. Restarting..." << std::endl;
        m_Restart = true;  // Restart when complete
    }
    if (m_Restart) {
        m_Simulation->Init();
        m_Restart = false;
        std::cout << "Simulation restarted." << std::endl;
    }
    if (!m_Simulation->IsComplete()) {
        m_Simulation->Step(static_cast<float>(dt));
        // Print stats every second
        static float lastPrint = 0.0f;
        float currentTime = m_Simulation->GetElapsedTime();
        if (currentTime - lastPrint >= 1.0f) {
            int remaining = m_Simulation->GetAgents().size();
            auto counts = m_Simulation->GetAgeCounts();
            std::cout << "Time: " << currentTime << "s, Remaining: " << remaining << " (C:" << counts[0]
                      << " Y:" << counts[1] << " A:" << counts[2] << " S:" << counts[3] << ")" << std::endl;
            lastPrint = currentTime;
        }
    }
}

void RescueApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_BodyShapes.Clear();

    // Define colors
    glm::vec4 colors[4] = {
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  // Yellow - Child
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // Green - Young
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  // Blue - Adult
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)   // Red - Senior
    };

    // Add shapes for agents
    const auto& agents = m_Simulation->GetAgents();
    for (const auto& agent : agents) {
        glm::vec4 color = colors[static_cast<int>(agent.traits.age)];
        // Body
        m_BodyShapes.Add(agent.Pos, agent.traits.bodyRadius * 2.0f, color);
    }

    // Render
    m_BodyShapes.Upload();
    m_BodyShapes.Draw(m_Shader, m_Camera.ViewProj(), 1.0f);
}
