#pragma once
namespace gr {

class Environment;

/**
 * Interface for collision handling
 */
class ICollisionHandler {
   public:
    virtual ~ICollisionHandler() = default;

    virtual void Resolve(Environment& env) = 0;
};

}  // namespace gr
