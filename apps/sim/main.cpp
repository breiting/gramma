#include <gramma/core/Runner.hpp>
#include <iostream>
#include <memory>

#include "SimApp.hpp"

using namespace gr;

int main() {
    auto app = std::make_unique<SimApp>();
    Runner runner;
    try {
        runner.Init(std::move(app), 1280, 720, "Simulation", 4);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    runner.Run();
    return 0;
}
