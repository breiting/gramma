#include <gramma/core/Window.hpp>

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

    m_LastTime = glfwGetTime();
    return true;
}

void Window::Destroy() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

bool Window::Poll() {
    if (!m_Window) return false;
    if (glfwWindowShouldClose(m_Window)) return false;

    double now = glfwGetTime();
    m_Dt = now - m_LastTime;
    m_LastTime = now;

    glfwPollEvents();
    glfwGetFramebufferSize(m_Window, &m_FramebufferWidth, &m_FramebufferHeight);
    glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);
    return true;
}

double Window::DeltaTime() const {
    return m_Dt;
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
