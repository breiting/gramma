#pragma once
namespace gramma {

/** Minimal application interface.
 *  Controller calls Update/Render each frame.
 */
class IApp {
   public:
    virtual ~IApp() = default;
    virtual const char* Name() const = 0;
    virtual bool Init() = 0;
    virtual void Update(double dt) = 0;
    virtual void Render() = 0;
};

}  // namespace gramma
