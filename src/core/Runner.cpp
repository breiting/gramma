#include <glad.h>

#include <gramma/core/Runner.hpp>
#include <gramma/core/Time.hpp>

namespace gr {

Runner::~Runner() {
    // ensure window/context are torn down after app is destroyed
    m_App.reset();
    m_Window.Destroy();
}

bool Runner::Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa) {
    if (!app) return false;

    if (!m_Window.Create({width, height, title, msaa})) return false;

    m_Quit = false;

    m_App = std::move(app);

    // Give the app a chance to set up GL resources
    return m_App->Init(*this);
}

void Runner::Run() {
    if (!m_App) return;

    const double dtUpdate = 1.0 / m_UpdateRate;
    double accumulator = 0.0;
    double lastTime = gr::Now();

    while (!m_Quit && Poll()) {
        double now = gr::Now();
        double frameTime = now - lastTime;
        lastTime = now;
        accumulator += frameTime;

        // Only update according to given update rate
        while (accumulator >= dtUpdate) {
            m_App->Update(*this, dtUpdate);
            accumulator -= dtUpdate;
        }

        BeginFrame();
        m_App->Render(*this);
        EndFrame();
    }
}

void Runner::SetUpdateRate(double rate) {
    m_UpdateRate = rate;
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
