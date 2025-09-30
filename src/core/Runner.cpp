#include <glad.h>

#include <gramma/core/Runner.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/view/Gui.hpp>

namespace gr {

Runner::~Runner() {
    // ensure window/context are torn down after app is destroyed
    m_App.reset();
    m_Window.Destroy();
}

void Runner::Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa) {
    if (!app) throw std::runtime_error("Invalid app");

    if (!m_Window.Create({width, height, title, msaa})) throw std::runtime_error("Window creation failed");

    m_Quit = false;

    m_App = std::move(app);

    if (!m_App->Init(m_Context)) {
        throw std::runtime_error("Application initialization failed");
    }

    m_Window.SetKeyPressedCallback(m_App->onKeyPressed);
    m_Window.SetMouseButtonCallback(m_App->onMouseButton);
    m_Window.SetMouseMoveCallback(m_App->onMouseMove);
}

void Runner::Run() {
    if (!m_App) return;

    Gui gui;
    // m_Gui.SetFontDirectory(fontDir);
    m_Window.RegisterGui(gui);

    const double dtUpdate = 1.0 / m_UpdateRate;
    double accumulator = 0.0;
    double lastTime = gr::Now();

    while (!m_Quit && Poll()) {
        double now = gr::Now();
        m_FrameDt = now - lastTime;
        lastTime = now;
        accumulator += m_FrameDt;

        // Only update according to given update rate
        while (accumulator >= dtUpdate) {
            m_UpdateDt = dtUpdate;
            m_App->Update(m_Context, dtUpdate);
            accumulator -= dtUpdate;
        }

        BeginFrame();
        gui.Begin();
        m_App->Render(m_Context);
        m_App->Ui(m_Context);
        gui.End();
        EndFrame();
    }
}

void Runner::SetUpdateRate(double rate) {
    m_UpdateRate = rate;
}

double Runner::GetFrameDt() {
    return m_FrameDt;
}

double Runner::GetUpdateDt() {
    return m_UpdateDt;
}

bool Runner::Poll() {
    if (!m_Window.Poll()) return false;
    return true;
}

void Runner::BeginFrame() {
    m_Window.BeginFrame();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Runner::EndFrame() {
    m_Window.EndFrame();
}

double Runner::Aspect() const {
    return m_Window.Aspect();
}

void Runner::RequestQuit() {
    m_Quit = true;
}

}  // namespace gr
