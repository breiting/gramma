#pragma once
#include <gramma/IApp.hpp>
#include <gramma/Window.hpp>
#include <memory>
#include <string>

namespace gramma {

/** Lightweight services the app may query without owning the window. */
class AppContext {
   public:
    struct WindowInfo {
        int fbWidth{1};
        int fbHeight{1};
        float aspect{1.0f};
    };

    virtual const WindowInfo& Window() const = 0;
    virtual double TimeNow() const = 0;
    virtual void RequestQuit() = 0;
    virtual ~AppContext() = default;
};

/** Minimal main loop + ownership of Window and App. */
class Runner : public AppContext {
   public:
    Runner() = default;
    ~Runner();

    /** Create window + GL context and take ownership of the app. */
    bool Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa = 4);

    /** Run the main loop (calls App::Init/Update/Render). */
    void Run();

    // --- AppContext impl ---
    const WindowInfo& Window() const override {
        return m_winInfo;
    }
    double TimeNow() const override {
        return m_now;
    }
    void RequestQuit() override {
        m_quit = true;
    }

   private:
    gramma::Window m_window;
    std::unique_ptr<IApp> m_app;

    WindowInfo m_winInfo{};
    double m_now{0.0};
    double m_last{0.0};
    bool m_quit{false};

    void beginFrame_();
    void endFrame_();
    bool poll_();
};

}  // namespace gramma
