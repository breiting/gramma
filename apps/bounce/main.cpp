#include <gramma/IApp.hpp>
#include <gramma/Runner.hpp>
#include <gramma/Window.hpp>
#include <memory>

#include "BounceApp.hpp"

using namespace gramma;

int main() {
    auto app = std::make_unique<BounceApp>();
    Runner runner;
    if (!runner.Init(std::move(app), 1280, 720, "gramma — Bounce")) return -1;
    runner.Run();
    return 0;
}
