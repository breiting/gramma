#include <gramma/core/Runner.hpp>
#include <iostream>
#include <memory>

#include "PlayApp.hpp"

using namespace gr;

int main() {
    auto app = std::make_unique<PlayApp>();
    Runner runner;
    try {
        runner.Init(std::move(app), 1280, 720, "Play Simulation", 4);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    runner.Run();
    return 0;
}
