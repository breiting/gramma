#pragma once
#include <gramma/model/ICollisionHandler.hpp>
#include <nanoflann.hpp>

namespace gr {

/**
 * Complexity: O(n log n + k), k = number of neighbors
 */
class KDTreeCollisionHandler : public ICollisionHandler {
   public:
    void Resolve(Environment& env) override;
};

}  // namespace gr
