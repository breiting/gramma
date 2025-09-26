#pragma once
// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

namespace gramma {

/** Thin GLFW window wrapper with basic frame control. */
class Window {
   public:
    struct CreateInfo {
        int width = 1200;
        int height = 800;
        std::string title = "gramma";
        int msaa = 4;
    };

    bool Create(const CreateInfo& ci);
    void Destroy();

    bool Poll();        // returns false if window should close
    void BeginFrame();  // clears
    void EndFrame();    // swap buffers

    GLFWwindow* Handle() const {
        return m_Window;
    }
    double DeltaTime() const {
        return m_Dt;
    }
    float Aspect() const {
        return float(m_FramebufferWidth) / float(m_FramebufferHeight);
    }

   private:
    GLFWwindow* m_Window = nullptr;
    double m_LastTime = 0.0, m_Dt = 0.0;
    int m_FramebufferWidth = 1, m_FramebufferHeight = 1;
};

}  // namespace gramma
