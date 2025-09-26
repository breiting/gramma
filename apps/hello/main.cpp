#include <gramma/IApp.hpp>
#include <gramma/Window.hpp>
#include <memory>

#include "HelloApp.hpp"

int main() {
    gramma::Window::CreateInfo ci;
    ci.title = "gramma – hello";
    ci.msaa = 4;

    gramma::Window win;
    if (!win.Create(ci)) return -1;

    std::unique_ptr<gramma::IApp> app = std::make_unique<HelloApp>();
    if (!app->Init()) return -1;

    while (win.Poll()) {
        win.BeginFrame();

        app->Update(win.DeltaTime());
        app->Render();

        win.EndFrame();
    }

    win.Destroy();
    return 0;
}
