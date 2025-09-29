#include <gramma/core/Window.hpp>
#include <stdexcept>

#include "imgui_impl_glfw.h"

namespace gr {

bool Window::Create(const CreateInfo& ci) {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, ci.msaa);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(ci.width, ci.height, ci.title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSwapInterval(1);
    glEnable(GL_MULTISAMPLE);

    glfwGetFramebufferSize(m_Window, &m_FramebufferWidth, &m_FramebufferHeight);
    glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);

    InstallGlfwCallbacks();

    return true;
}

void Window::Destroy() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

void Window::InstallGlfwCallbacks() {
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetKeyCallback(m_Window, [](GLFWwindow* w, int key, int sc, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self) return;

        if (self->m_KeyPressedHandler && action == GLFW_PRESS) {
            self->m_KeyPressedHandler->HandleKeyPressedEvent(key, mods);
        }
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* w, int button, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self) return;

        if (self->m_MouseButtonHandler) {
            self->m_MouseButtonHandler->HandleMouseButtonEvent(button, action, mods);
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* w, double xoff, double yoff) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        // self->m_Camera->OnScrollWheel(yoff);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* w, double x, double y) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        if (self->m_MouseMoveHandler) {
            self->m_MouseMoveHandler->HandleMouseMoveEvent(x, y);
        }
    });
}

bool Window::Poll() {
    if (!m_Window) return false;
    if (glfwWindowShouldClose(m_Window)) return false;

    glfwPollEvents();
    glfwGetFramebufferSize(m_Window, &m_FramebufferWidth, &m_FramebufferHeight);
    glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);
    return true;
}

void Window::RegisterGui(Gui& gui) {
    gui.Initialize(m_Window);
}

float Window::Aspect() const {
    return float(m_FramebufferWidth) / float(m_FramebufferHeight);
}

void Window::BeginFrame() {
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::EndFrame() {
    glfwSwapBuffers(m_Window);
}

}  // namespace gr
