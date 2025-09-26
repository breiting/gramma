#pragma once
#include <glm/vec2.hpp>

namespace gramma {

/** Rigid 2D circle (world units = meters). */
struct Body2D {
    glm::vec2 pos{0.0f};      // m
    glm::vec2 vel{0.0f};      // m/s
    float radius{0.1f};       // m
    float invMass{1.0f};      // 1/kg (0 = static)
    float restitution{0.6f};  // [0..1] bounciness

    static Body2D Dynamic(float mass, float radius, float restitution) {
        Body2D b;
        b.radius = radius;
        b.invMass = (mass > 0.f ? 1.f / mass : 0.f);
        b.restitution = restitution;
        return b;
    }
    static Body2D Static(float radius = 0.f) {
        Body2D b;
        b.radius = radius;
        b.invMass = 0.f;
        b.restitution = 0.f;
        return b;
    }
};

}  // namespace gramma
