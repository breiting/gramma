#include "ParticleApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/particle/ParticleLifeBehavior.hpp>
#include <gramma/model/particle/ParticleSystem.hpp>
#include <gramma/model/particle/SimpleParticleBehavior.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

using namespace gr;
using namespace std;

static constexpr float AttractionMatrix[4][4] = {
    {+0.8f, -0.6f, +0.2f, -0.3f},  // Group 0 (z.B. "Red")
    {-0.4f, +0.9f, -0.2f, +0.1f},  // Group 1 (Green)
    {+0.3f, -0.3f, +0.7f, -0.5f},  // Group 2 (Blue)
    {-0.2f, +0.1f, -0.5f, +0.9f}   // Group 3 (Yellow)
};

std::string ParticleApp::Name() const {
    return "ParticleApp";
}

void ParticleApp::GenerateParticles(int count, float radius, int groups) {
    m_System->Init(count, radius, groups);
}

bool ParticleApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing ParticleApp..." << std::endl;

    constexpr float border = 1.0;
    constexpr float globalWidth = 100.0;
    constexpr float globalHeight = 100.0;
    constexpr float numParticles = 2000;
    constexpr float particleRadius = 0.4;
    constexpr float neighborhoodRadius = 3.0;
    constexpr float cellSize = 2.0;
    constexpr int numGroups = 2;

    auto height = globalHeight + border;

    std::vector<std::vector<float>> matrix;
    for (int i = 0; i < 3; ++i) {
        std::vector<float> row;
        for (int j = 0; j < 3; ++j) {
            row.push_back(AttractionMatrix[i][j]);
        }
        matrix.push_back(row);
    }

    const std::vector<std::vector<float>> attractionMatrix = {
        {10.5f, -1.0f},
        {-1.0f, 10.5f},
    };

    m_System = std::make_unique<gr::ParticleSystem>(globalWidth, globalHeight, cellSize);
    m_System->SetBehavior(std::make_unique<ParticleLifeBehavior>(neighborhoodRadius, attractionMatrix));
    // m_System->SetBehavior(std::make_unique<SimpleParticleBehavior>(neighborhoodRadius, 2.0));

    GenerateParticles(numParticles, particleRadius, numGroups);

    m_View.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    m_Camera.SetOrthoByHeight(height, ctx.Aspect());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
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
        m_Camera.FitToEnvironment(nullptr, float(w) / float(h));  //
    };

    std::cout << "ParticleApp initialized." << std::endl;
    return true;
}

void ParticleApp::Update(gr::AppContext& /*ctx*/, double dt) {
    {
        TimeMeasureGuard guard("Step");
        m_System->Step(static_cast<float>(dt) * m_Timescale);
    }

    {
        TimeMeasureGuard guard("Sync");
        m_View.SyncWithParticleSystem(m_System.get());
    }
}

void ParticleApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_Gui->BeginFrame();

    ImGui::Begin("Environment Stats");
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.5, 10.0);
    ImGui::End();

    m_Gui->EndFrame();

    m_View.Draw(m_System.get(), m_Camera);
}
