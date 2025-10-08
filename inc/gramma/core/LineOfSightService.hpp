
#pragma once

#include <glm/glm.hpp>
#include <gramma/core/Math.hpp>
#include <vector>

namespace gr {

class LineOfSightService {
   public:
    static bool CanSee(const glm::vec2& from, const glm::vec2& to, const std::vector<Polygon>& obstacles);

   private:
    static bool DoSegmentsIntersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2);
    static int Orientation(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r);
    static bool OnSegment(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r);
};
}  // namespace gr
