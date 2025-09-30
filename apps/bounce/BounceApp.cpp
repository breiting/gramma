#include "BounceApp.hpp"

#include <cstdio>
#include <cstdlib>
#include <gramma/core/Runner.hpp>
#include <vector>

#include "GLFW/glfw3.h"
#include "gramma/model/AgentTraits.hpp"

using namespace gr;

std::string BounceApp::Name() const {
    return "BallBounce";
}

bool BounceApp::Init(gr::AppContext& ctx) {
    // View
    m_Camera.SetOrthoByHeight(10, ctx.Aspect());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Circles.Init();
    m_Lines.Init();
    m_CircleShader.BuildCircle();
    m_LineShader.BuildLine();

    // Model
    m_World.gravity = {0.f, -9.81f};
    m_World.groundY = -0.6f;
    m_World.groundFriction = 0.15f;
    // one dynamic ball (mass=1kg, r=0.1m, e=0.65)

    for (int i = -5; i < 6; i++) {
        auto idx = m_World.addBody(Body2D::Dynamic(0.1f, 0.10f, abs(i * 0.1) + 0.3));
        m_World.body(idx).pos = {i, 5.f};
        m_World.body(idx).vel = {0.0f, 0.0f};
    }

    // ground line for rendering
    m_Lines.Set({{-10.f, m_World.groundY}, {10.f, m_World.groundY}});

    // Set input callbacks
    onKeyPressed = [this](int key, int mods) {
        if (key == GLFW_KEY_ESCAPE) {
            m_Quit = true;
        }
    };
    onMouseButton = [this](int button, int action, int mods) { printf("%d\n", button); };
    // onMouseMove = [this](double x, double y) { printf("%3.2f %3.2f\n", x, y); };

    // Load font
    // if (!m_font.Load("../assets/fonts/Roboto-Medium.ttf", 24.0f)) {
    //     return false;
    // }

    // Create HUD panel
    // auto* hud = ctx.GetHud();
    // m_PerfPanel = std::make_unique<Panel>(glm::vec2(10, 10), glm::vec2(250, 80), hud->GetQuadBatch(),
    //                                       glm::vec4(0.5, 0, 0, 0.7), glm::vec4(0.5, 1, 1, 1), 2.0f);

    // m_fpsText = std::make_unique<TextElement>(glm::vec2(20, 20), "Bernhard", glm::vec4(1, 1, 1, 1), &m_font);
    // m_PerfPanel->AddElement(std::move(m_fpsText));

    // hud->AddPanel(std::move(m_PerfPanel));

    m_AgentView.Init();

    m_Agent.Pos = {1.0, 1.0};
    AgentTraits traits;
    traits.age = gr::AgeClass::Child;
    traits.bodyRadius = 0.2;
    traits.comfortRadius = 0.8;
    m_Agent.traits = traits;

    return true;
}

void BounceApp::Update(gr::AppContext& ctx, double dt) {
    m_World.Step((float)dt);
}

void BounceApp::Render(gr::AppContext& ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    glm::mat4 vp = m_Camera.ViewProj();

    // circles
    m_Circles.Clear();
    for (auto& b : m_World.bodies()) {
        m_Circles.Add(b.pos, 2.0f * b.radius);
    }
    m_Circles.Upload();
    m_Lines.Draw(m_LineShader, vp, glm::vec4(1.0, 1.0, 1.0, 0.8));
    m_Circles.Draw(m_CircleShader, vp, glm::vec4(1.0, 1.0, 1.0, 0.95));

    m_AgentView.Draw(m_Agent, vp);
}
