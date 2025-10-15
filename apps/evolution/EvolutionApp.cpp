#include "EvolutionApp.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <gramma/core/AppContext.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/core/Window.hpp>
#include <gramma/model/particle/NeuralParticleBehavior.hpp>
#include <gramma/ui/ImGuiLayer.hpp>
#include <iostream>
#include <memory>

using namespace gr;
using namespace std;

std::string EvolutionApp::Name() const {
    return "EvolutionApp";
}

bool EvolutionApp::Init(gr::AppContext& ctx) {
    std::cout << "Initializing EvolutionApp..." << std::endl;

    m_Population = std::make_unique<Population>(m_NumParticles, 2, 8, 2);
    m_Evaluator = std::make_unique<CircleFormationEvaluator>(10.f);
    m_System = std::make_unique<ParticleSystem>(m_GlobalWidth, m_GlobalHeight, m_CellSize);

    m_System->Init(m_NumParticles, m_ParticleRadius, m_NumGroups);
    m_System->SetBehavior(std::make_unique<NeuralParticleBehavior>(10.0));

    for (size_t i = 0; i < m_System->GetParticles().size(); i++) {
        m_System->GetParticles()[i]->SetGenome(&m_Population->GetGenomes()[i]);
    }

    m_Camera.SetOrthoByHeight(m_GlobalHeight, ctx.Aspect());

    m_View.Init();

    m_Gui = std::make_unique<ImGuiLayer>(ctx.GetWindow().GetNativeWindow());

    m_Camera.SetOrthoByHeight(100, ctx.Aspect());

    onKeyPressed = [this](int key, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        }
    };

    onScroll = [this, &ctx](double /*xoffs*/, double yoffs) {
        m_Camera.ZoomAtCursor(yoffs * 0.1f, m_MousePos, ctx.GetWidth(), ctx.GetHeight());
    };

    onMouseMove = [this](double x, double y) {
        auto pos = glm::vec2(x, y);  //
        if (m_IsDragging) {
            glm::vec2 delta = pos - m_MousePos;
            delta.y *= -1.0;
            m_Camera.Pan(-delta * 0.2f);
        }
        m_MousePos = pos;
    };

    onMouseButton = [this](int button, int action, int /*mod*/) {
        if (button == GLFW_MOUSE_BUTTON_1) {
            if (action == GLFW_PRESS) {
                m_DragStart = m_MousePos;
                m_IsDragging = true;
            } else if (action == GLFW_RELEASE) {
                m_IsDragging = false;
            }
        }
    };

    onWindowSize = [this](int w, int h) {
        m_Camera.FitToEnvironment(nullptr, float(w) / float(h));  //
    };

    std::cout << "EvolutionApp initialized." << std::endl;
    return true;
}

void EvolutionApp::Update(gr::AppContext& /*ctx*/, double dt) {
    m_System->Step(static_cast<float>(dt) * m_Timescale);
    m_CurrentStep++;

    if (m_CurrentStep > m_GenerationSteps) {
        m_Evaluator->Evaluate(*m_System);
        m_Population->Evolve();

        // assign new genomes
        auto& gens = m_Population->GetGenomes();
        auto& parts = m_System->GetParticles();
        for (size_t i = 0; i < parts.size(); ++i) parts[i]->SetGenome(&gens[i]);

        m_System->RandomizePositions();
        m_CurrentStep = 0;
        m_Generation++;
    }

    m_View.SyncWithParticleSystem(m_System.get());
    m_Camera.Update(dt);
}

void EvolutionApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    m_View.Draw(m_System.get(), m_Camera);

    m_Gui->BeginFrame();

    ImGui::Begin("Evolution");
    ImGui::Text("Generation: %d", m_Generation);
    ImGui::Text("Particles: %zu", m_System->GetParticles().size());
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.1f, 10.0f);
    ImGui::SliderInt("Generation Steps", &m_GenerationSteps, 300, 800);
    if (ImGui::Button("SAVE")) {
        m_Population->GetBestGenome().Save("genome.json");
    }
    ImGui::End();

    m_Gui->EndFrame();
}
