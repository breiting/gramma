#pragma once
// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

namespace gr {

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

    double DeltaTime() const;
    float Aspect() const;

   private:
    GLFWwindow* m_Window;
    double m_LastTime;
    double m_Dt;
    int m_FramebufferWidth;
    int m_FramebufferHeight;
};

}  // namespace gr
