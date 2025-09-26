#pragma once
#include <string>

namespace gramma {

class AppContext;

/** Minimal application interface.
 *  Controller calls Update/Render each frame.
 */
class IApp {
   public:
    virtual ~IApp() = default;
    virtual std::string Name() const = 0;
    virtual bool Init(AppContext& ctx) = 0;
    virtual void Update(AppContext& ctx, double dt) = 0;
    virtual void Render(AppContext& ctx) = 0;
};

}  // namespace gramma
