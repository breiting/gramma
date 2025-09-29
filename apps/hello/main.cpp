#include <gramma/core/IApp.hpp>
#include <gramma/core/Runner.hpp>
#include <gramma/core/Window.hpp>
#include <memory>

#include "HelloApp.hpp"

using namespace gr;

int main() {
    auto app = std::make_unique<HelloApp>();
    Runner runner;
    if (!runner.Init(std::move(app), 1280, 720, "gramma — Hello")) {
        return -1;
    }
    runner.Run();
    return 0;
}
