#pragma once
// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <functional>
#include <string>

#include "gramma/core/IApp.hpp"
#include "gramma/view/Gui.hpp"

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

    float Aspect() const;

    void RegisterGui(Gui& gui);

    void SetKeyPressedHandler(IKeyPressedHandler* h) {
        m_KeyPressedHandler = h;
    }

    void SetMouseButtonHandler(IMouseButtonHandler* h) {
        m_MouseButtonHandler = h;
    }

    void SetMouseMoveHandler(IMouseMoveHandler* h) {
        m_MouseMoveHandler = h;
    }

   private:
    void InstallGlfwCallbacks();

   private:
    GLFWwindow* m_Window;

    // Handler
    IKeyPressedHandler* m_KeyPressedHandler = nullptr;
    IMouseButtonHandler* m_MouseButtonHandler = nullptr;
    IMouseMoveHandler* m_MouseMoveHandler = nullptr;

    int m_FramebufferWidth;
    int m_FramebufferHeight;
};

}  // namespace gr
