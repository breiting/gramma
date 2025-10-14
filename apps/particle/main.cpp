#include <gramma/core/Runner.hpp>
#include <iostream>
#include <memory>

#include "ParticleApp.hpp"

using namespace gr;

int main() {
    auto app = std::make_unique<ParticleApp>();
    Runner runner;
    try {
        runner.Init(std::move(app), 1280, 720, "Particle Simulation", 4);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    runner.Run();
    return 0;
}
