#pragma once
#include <string>

namespace gr {

class Agent;
class Environment;

/** Abstract interface for agent needs (hunger, fatigue, safety, etc.) */
class INeed {
   public:
    virtual ~INeed() = default;

    /// Update internal state (for time-driven needs like hunger)
    virtual void Update(float /*dt*/) {
    }

    /// Evaluate current urgency in context (for environment-driven needs)
    virtual float Evaluate(const Agent& /*self*/, const Environment& /*env*/) const {
        return Priority();
    }

    virtual float Utility(const Agent& /*self*/, const Environment& /*env*/) const {
        return Priority();
    }

    /// Urgency [0..1]
    virtual float Priority() const {
        return 0.0f;
    }

    /// Name of the need
    virtual std::string Name() const = 0;

    /// Reset when satisfied
    virtual void Reset() {
    }
};

}  // namespace gr
