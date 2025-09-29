#include <glad.h>

#include <gramma/core/Runner.hpp>

namespace gr {

Runner::~Runner() {
    // ensure window/context are torn down after app is destroyed
    m_App.reset();
    m_Window.Destroy();
}

bool Runner::Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa) {
    if (!app) return false;

    if (!m_Window.Create({width, height, title, msaa})) return false;

    m_Now = 0.0;
    m_LastTime = 0.0;
    m_Quit = false;

    m_App = std::move(app);

    // Give the app a chance to set up GL resources
    return m_App->Init(*this);
}

void Runner::Run() {
    if (!m_App) return;

    while (!m_Quit && Poll()) {
        BeginFrame();

        // App update & render
        m_App->Update(*this, m_Window.DeltaTime());
        m_App->Render(*this);

        EndFrame();
    }
}

bool Runner::Poll() {
    if (!m_Window.Poll()) return false;
    return true;
}

void Runner::BeginFrame() {
    m_Window.BeginFrame();
    // common GL state you want globally (optional)
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
