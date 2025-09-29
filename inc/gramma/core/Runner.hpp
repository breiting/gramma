#pragma once
#include <gramma/core/AppContext.hpp>
#include <gramma/core/IApp.hpp>
#include <gramma/core/Window.hpp>
#include <memory>
#include <string>

namespace gr {

class Runner : public AppContext {
   public:
    Runner() = default;
    ~Runner();

    /** Create window + GL context and take ownership of the app. */
    bool Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa = 4);

    void Run();

    void SetUpdateRate(double rate);  // in Hz (e.g. 60)

    // AppContext
    double Aspect() const override;
    void RequestQuit() override;

   private:
    gr::Window m_Window;
    std::unique_ptr<IApp> m_App;

    double m_UpdateRate{60.0};
    bool m_Quit{false};

    void BeginFrame();
    void EndFrame();
    bool Poll();
};

}  // namespace gr
