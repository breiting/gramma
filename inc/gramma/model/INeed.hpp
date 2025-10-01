#pragma once
#include <string>

namespace gr {

/** Abstract interface for agent needs (hunger, fatigue, safety, etc.) */
class INeed {
   public:
    virtual ~INeed() = default;

    virtual void Update(float dt) = 0;     // update internal value
    virtual float Priority() const = 0;    // [0..1], urgency
    virtual std::string Name() const = 0;  // name of the need
    virtual void Reset() = 0;              // reset when satisfied
};

}  // namespace gr
