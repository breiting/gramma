#pragma once
#include <string>

namespace gr {

class AppContext;

class IKeyPressedHandler {
   public:
    virtual void HandleKeyPressedEvent(int key, int mods) = 0;
};

class IMouseButtonHandler {
   public:
    virtual void HandleMouseButtonEvent(int button, int action, int mods) = 0;
};

class IMouseMoveHandler {
   public:
    virtual void HandleMouseMoveEvent(double x, double y) = 0;
};

/** Minimal application interface.
 */
class IApp : public IKeyPressedHandler, public IMouseButtonHandler, public IMouseMoveHandler {
   public:
    virtual ~IApp() = default;
    virtual std::string Name() const = 0;
    virtual bool Init(AppContext& ctx) = 0;
    virtual void Update(AppContext& ctx, double dt) = 0;
    virtual void Render(AppContext& ctx) = 0;
    virtual void Ui(AppContext& ctx) = 0;

    void HandleKeyPressedEvent(int key, int mods) override {
        // no implementation
    }
    void HandleMouseButtonEvent(int button, int action, int mods) override {
        // no implementation
    }
    void HandleMouseMoveEvent(double x, double y) override {
        // no implementation
    }
};

}  // namespace gr
