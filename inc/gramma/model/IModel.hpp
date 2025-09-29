#pragma once

#include <cstdint>

namespace gr {
class IModel {
   public:
    virtual ~IModel() = default;
    virtual void Reset(uint32_t seed) = 0;
    virtual void Step(double dt) = 0;
};
}  // namespace gr
