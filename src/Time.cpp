#include <GLFW/glfw3.h>

#include <gramma/Time.hpp>

namespace gramma {
void Time::Tick() {
    double t = glfwGetTime();
    dt = t - now;
    now = t;
}
}  // namespace gramma
