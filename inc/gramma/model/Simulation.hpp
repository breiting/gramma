#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Exit.hpp>
#include <gramma/model/ICollisionHandler.hpp>
#include <gramma/model/INavigationStrategy.hpp>
#include <gramma/model/Room.hpp>

namespace gr {

/** Manages the evacuation simulation. */
class Simulation {
   public:
    Simulation(const Room& room, const Exit& exit, std::unique_ptr<INavigationStrategy> navStrategy,
               std::unique_ptr<ICollisionHandler> collisionHandler, int numAgents);
    ~Simulation() = default;

    /** Initialize the simulation with agents. */
    void Init();

    /** Perform one simulation step with given time delta. */
    void Step(float dt);

    /** Check if the simulation is complete (all agents exited). */
    bool IsComplete() const;

    /** Get the current agents (for rendering). */
    const std::vector<Agent>& GetAgents() const;

    /** Get the room. */
    const Room& GetRoom() const;

    /** Get the exit. */
    const Exit& GetExit() const;

   private:
    Room m_Room;
    Exit m_Exit;
    std::unique_ptr<INavigationStrategy> m_NavStrategy;
    std::unique_ptr<ICollisionHandler> m_CollisionHandler;
    std::vector<Agent> m_Agents;
    int m_NumAgents;
};

}  // namespace gr