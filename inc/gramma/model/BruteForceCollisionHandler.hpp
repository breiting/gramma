#pragma once
#include <gramma/model/ICollisionHandler.hpp>

namespace gr {

/**
 * Complexity: O(n^2)
 */
class BruteForceCollisionHandler : public ICollisionHandler {
   public:
    void Resolve(Environment& env) override;
};

}  // namespace gr
