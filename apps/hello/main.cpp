#include <gramma/core/IApp.hpp>
#include <gramma/core/Runner.hpp>
#include <gramma/core/Window.hpp>
#include <iostream>
#include <memory>

#include "HelloApp.hpp"

using namespace gr;

int main() {
    auto app = std::make_unique<HelloApp>();
    Runner runner;
    try {
        runner.Init(std::move(app), 1280, 720, "gramma — Hello", 4);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    runner.Run();
    return 0;
}
