#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "Body2D.hpp"
#include "gramma/model/IModel.hpp"

namespace gr {

/** Minimal 2D physics world: gravity, ground (y = groundY), semi-implicit Euler. */
class World2D : public IModel {
   public:
    static constexpr float defaultGravity = -9.81f;
    static constexpr float defaultGroundY = 0.0f;
    static constexpr float defaultGroundFriction = 0.0f;

    glm::vec2 gravity{0.0f, defaultGravity};      // m/s^2
    float groundY{defaultGroundY};                // floor line at y = groundY
    float groundFriction{defaultGroundFriction};  // optional tangential damping on bounce [0..1]

    /** Add a body to the world.
     * @param b The body to add.
     * @return The index of the added body.
     */
    size_t addBody(const Body2D& b) {
        m_bodies.push_back(b);
        return m_bodies.size() - 1;
    }

    /** Get a mutable reference to a body.
     * @param i The index of the body.
     * @return Reference to the body.
     */
    Body2D& body(size_t i) {
        return m_bodies[i];
    }

    /** Get a const reference to a body.
     * @param i The index of the body.
     * @return Const reference to the body.
     */
    const Body2D& body(size_t i) const {
        return m_bodies[i];
    }

    /** Get all bodies.
     * @return Const reference to the vector of bodies.
     */
    const std::vector<Body2D>& bodies() const {
        return m_bodies;
    }

    /** Step the physics simulation.
     * @param dt Time delta.
     */
    void Step(double dt);

   private:
    std::vector<Body2D> m_bodies;

    void integrate(Body2D& b, float dt);
    void solveGroundCollision(Body2D& b);
};

}  // namespace gr
