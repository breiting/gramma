#include <gramma/core/IApp.hpp>
#include <gramma/core/Runner.hpp>
#include <gramma/core/Window.hpp>
#include <memory>

#include "BounceApp.hpp"

using namespace gr;

int main() {
    auto app = std::make_unique<BounceApp>();
    Runner runner;
    if (!runner.Init(std::move(app), 1280, 720, "gramma — Bounce")) return -1;
    runner.Run();
    return 0;
}
