#pragma once

namespace gr {

/** Simple interface which allows the app to interact with other components.
 * The runner typically implements this class.
 * */
struct AppContext {
   public:
    virtual double Aspect() const = 0;
    virtual void RequestQuit() = 0;
};

}  // namespace gr
