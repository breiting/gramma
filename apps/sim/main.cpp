#include <gramma/core/Runner.hpp>
#include <iostream>
#include <memory>

#include "SegregationApp.hpp"
#include "SimApp.hpp"

using namespace gr;

int main() {
    std::string name = "seg";
    std::unique_ptr<IApp> app;

    if (name == "sim") {
        app = std::make_unique<SimApp>();
    } else if (name == "seg") {
        app = std::make_unique<SegregationApp>();
    }

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
