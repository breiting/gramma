#include <gramma/core/LineOfSightService.hpp>

namespace gr {

bool LineOfSightService::CanSee(const glm::vec2& from, const glm::vec2& to, const std::vector<Polygon>& obstacles) {
    for (const auto& poly : obstacles) {
        const auto& vertices = poly.vertices;
        for (size_t i = 0; i < vertices.size(); ++i) {
            glm::vec2 p1 = vertices[i];
            glm::vec2 p2 = vertices[(i + 1) % vertices.size()];
            if (DoSegmentsIntersect(from, to, p1, p2)) {
                return false;
            }
        }
    }
    return true;
}

bool LineOfSightService::DoSegmentsIntersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1,
                                             const glm::vec2& q2) {
    int o1 = Orientation(p1, p2, q1);
    int o2 = Orientation(p1, p2, q2);
    int o3 = Orientation(q1, q2, p1);
    int o4 = Orientation(q1, q2, p2);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && OnSegment(p1, q1, p2)) return true;
    if (o2 == 0 && OnSegment(p1, q2, p2)) return true;
    if (o3 == 0 && OnSegment(q1, p1, q2)) return true;
    if (o4 == 0 && OnSegment(q1, p2, q2)) return true;

    return false;
}

int LineOfSightService::Orientation(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r) {
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool LineOfSightService::OnSegment(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r) {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) &&
           q.y >= std::min(p.y, r.y);
}
}  // namespace gr
