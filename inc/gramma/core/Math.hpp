#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace gr {

constexpr double PI = 3.14159265358979323846;

struct LineSegment {
    glm::vec2 a, b;
};

struct Polygon {
    std::vector<glm::vec2> vertices;
};

}  // namespace gr
