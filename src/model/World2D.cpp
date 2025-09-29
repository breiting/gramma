#include <algorithm>
#include <cmath>
#include <gramma/model/World2D.hpp>

namespace gr {

void World2D::Reset(uint32_t seed) {
}

void World2D::Step(double dt) {
    for (auto& b : m_bodies) {
        integrate(b, dt);
        solveGroundCollision(b);
    }
}

void World2D::integrate(Body2D& b, float dt) {
    if (b.invMass <= 0.f) return;  // static
    // v_{t+dt} = v_t + dt * g
    b.vel += gravity * dt;
    // x_{t+dt} = x_t + dt * v_{t+dt}  (semi-implicit Euler; stable for gravity)
    b.pos += b.vel * dt;
}

void World2D::solveGroundCollision(Body2D& b) {
    // only circle vs infinite horizontal line y=groundY
    float pen = (b.pos.y - b.radius) - groundY;  // penetration >0 means above; <0 penetrates
    if (pen < 0.0f) {
        // positional correction
        b.pos.y = groundY + b.radius;
        // reflect normal component with restitution
        float vn = b.vel.y;
        if (vn < 0.0f) {
            b.vel.y = -b.restitution * vn;
            // simple ground friction: damp x-velocity on bounce
            b.vel.x *= (1.0f - std::clamp(groundFriction, 0.0f, 1.0f));
            // sleep threshold (stop jitter)
            if (std::abs(b.vel.y) < 0.05f) b.vel.y = 0.0f;
            if (std::abs(b.vel.x) < 0.02f) b.vel.x = 0.0f;
        }
    }
}

}  // namespace gr
