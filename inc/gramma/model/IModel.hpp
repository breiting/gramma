#pragma once

#include <cstdint>

namespace gr {
class IModel {
   public:
    virtual ~IModel() = default;
    virtual void Reset(uint32_t /*seed*/) {
    }
    virtual void Step(double dt) = 0;
};
}  // namespace gr
