#include <GLFW/glfw3.h>

#include <gramma/core/Time.hpp>

namespace gr {
void Time::Tick() {
    double t = glfwGetTime();
    dt = t - now;
    now = t;
}
}  // namespace gr
