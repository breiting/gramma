#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "Body2D.hpp"
#include "gramma/model/IModel.hpp"

namespace gr {

/** Minimal 2D physics world: gravity, ground (y = groundY), semi-implicit Euler. */
class World2D : public IModel {
   public:
    glm::vec2 gravity{0.0f, -9.81f};  // m/s^2
    float groundY{0.0f};              // floor line at y = groundY
    float groundFriction{0.0f};       // optional tangential damping on bounce [0..1]

    size_t addBody(const Body2D& b) {
        m_bodies.push_back(b);
        return m_bodies.size() - 1;
    }
    Body2D& body(size_t i) {
        return m_bodies[i];
    }
    const Body2D& body(size_t i) const {
        return m_bodies[i];
    }
    const std::vector<Body2D>& bodies() const {
        return m_bodies;
    }

    void Reset(uint32_t seed);
    void Step(double dt);

   private:
    std::vector<Body2D> m_bodies;

    void integrate(Body2D& b, float dt);
    void solveGroundCollision(Body2D& b);
};

}  // namespace gr
