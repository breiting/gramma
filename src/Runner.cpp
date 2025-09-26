#include <glad.h>

#include <gramma/Runner.hpp>

namespace gramma {

Runner::~Runner() {
    // ensure window/context are torn down after app is destroyed
    m_app.reset();
    m_window.Destroy();
}

bool Runner::Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa) {
    if (!app) return false;

    gramma::Window::CreateInfo ci;
    ci.width = width;
    ci.height = height;
    ci.title = title;
    ci.msaa = msaa;

    if (!m_window.Create(ci)) return false;

    // initial window info
    m_winInfo.fbWidth = 1;
    m_winInfo.fbHeight = 1;
    m_winInfo.aspect = m_window.Aspect();

    m_now = 0.0;
    m_last = 0.0;
    m_quit = false;

    m_app = std::move(app);

    // Give the app a chance to set up GL resources
    return m_app->Init(*this);
}

void Runner::Run() {
    if (!m_app) return;

    while (!m_quit && poll_()) {
        beginFrame_();

        // App update & render
        m_app->Update(*this, m_window.DeltaTime());
        m_app->Render(*this);

        endFrame_();
    }
}

bool Runner::poll_() {
    if (!m_window.Poll()) return false;

    // refresh framebuffer size / aspect for context queries
    // Window already updated viewport in Poll()
    // expose via AppContext
    // We fetch size via GLFW since Window already keeps it for Aspect()
    // but we only need aspect here:
    m_winInfo.aspect = m_window.Aspect();

    // Approximate absolute time using GLFW's timer (via Window::Poll dt integration)
    m_now += m_window.DeltaTime();
    return true;
}

void Runner::beginFrame_() {
    m_window.BeginFrame();
    // common GL state you want globally (optional)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Runner::endFrame_() {
    m_window.EndFrame();
}

}  // namespace gramma
