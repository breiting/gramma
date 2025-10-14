#include "ParticleApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/particle/ParticleLifeBehavior.hpp>
#include <gramma/model/particle/ParticleSystem.hpp>
#include <gramma/model/particle/RepulsionOnlyBehavior.hpp>
#include <gramma/model/particle/SimpleParticleBehavior.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

using namespace gr;
using namespace std;

constexpr float border = 1.0;
constexpr float globalWidth = 100.0;
constexpr float globalHeight = 100.0;
constexpr float numParticles = 1000;
constexpr float particleRadius = 0.4;
constexpr float neighborhoodRadius = 2.0;
constexpr float cellSize = 1.0;
constexpr int numGroups = 2;

static constexpr float AttractionMatrix[4][4] = {
    {+0.8f, -50.6f, +10.2f, -0.3f},  // Group 0 (z.B. "Red")
    {-50.4f, +0.9f, -0.2f, +0.1f},   // Group 1 (Green)
    {+10.3f, -0.3f, +0.7f, -0.5f},   // Group 2 (Blue)
    {-0.2f, +0.1f, -0.5f, +0.9f}     // Group 3 (Yellow)
};

std::string ParticleApp::Name() const {
    return "ParticleApp";
}

void ParticleApp::GenerateParticles(int count, float radius, int groups) {
    m_System->Init(count, radius, groups);
    m_ParticleIdCounter += count;
#if 0
    int id = 0;
    int w = 50;
    int h = 50;
    for (int x = -w / 2; x < w / 2; x++) {
        for (int y = -h / 2; y < h / 2; y++) {
            auto pos = glm::vec2(x, y);
            auto p = std::make_unique<Particle>(std::to_string(id++), pos, 0, 0.25);
            m_System->AddParticle(std::move(p));
        }
    }
#endif
}

bool ParticleApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing ParticleApp..." << std::endl;

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
        {1.5f, -10.0f},
        {-10.0f, 1.5f},
    };

    m_System = std::make_unique<gr::ParticleSystem>(globalWidth, globalHeight, cellSize);
    m_System->SetBehavior(std::make_unique<ParticleLifeBehavior>(neighborhoodRadius, matrix));
    // m_System->SetBehavior(std::make_unique<RepulsionOnlyBehavior>(neighborhoodRadius, 6.0, 0.95));
    // m_System->SetBehavior(std::make_unique<SimpleParticleBehavior>(neighborhoodRadius, 1.0));

    // GenerateParticles(numParticles, particleRadius, numGroups);

    m_View.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    m_Camera.SetOrthoByHeight(height, ctx.Aspect());

    onKeyPressed = [this, &ctx](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        } else if (key == GLFW_KEY_A) {
            for (int i = 0; i < 10; i++) {
                auto pos = glm::vec2(0, 0);
                auto p = std::make_unique<Particle>(std::to_string(m_ParticleIdCounter++), pos, 0, 0.25);
                m_System->AddParticle(std::move(p));
            }
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
    if (m_ParticleIdCounter < 1000) {
        std::uniform_real_distribution<float> velo(0, glm::two_pi<float>());
        std::uniform_int_distribution<int> group(0, 2);

        auto pos = glm::vec2(0, 0);
        auto p = std::make_unique<Particle>(std::to_string(m_ParticleIdCounter++), pos, group(m_Rng), 0.25);
        float angle = velo(m_Rng);
        glm::vec2 v = glm::vec2(glm::cos(angle), glm::sin(angle)) * 1.0f;
        p->SetVelocity(v);
        m_System->AddParticle(std::move(p));
    }
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

    m_View.Draw(m_System.get(), m_Camera);

    m_Gui->BeginFrame();

    ImGui::Begin("Environment Stats");
    ImGui::Text("Particles: %zu", m_System->GetParticles().size());
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.5, 10.0);
    ImGui::SliderInt("#Particles", &m_NumParticles, 10, 10000);
    if (ImGui::Button("Start")) {
        // GenerateParticles(m_NumParticles, particleRadius, numGroups);
        m_System->Clear();
        m_ParticleIdCounter = 0;
    }
    ImGui::End();

    m_Gui->EndFrame();
}
