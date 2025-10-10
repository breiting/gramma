# Agents

## What is an Agent?

An **agent** is an autonomous, decision-making entity situated within a simulated environment. It perceives its surroundings, makes decisions based on internal state and external inputs, and executes actions accordingly. Agents can represent individuals, machines, animals, vehicles, or abstract entities like information packets or financial assets—depending on the model's purpose.

### Examples

- A human evacuating a building during a fire
- A worker navigating a production floor
- A vehicle in a traffic simulation
- A customer in a retail queue
- A predator or prey in an ecological system

Agents are the atomic units of behavior in **Agent-Based Models (ABMs)**. Their individual actions, interactions, and local rules lead to emergent system-level behavior.

## Agent-Based Modeling (ABM)

**Agent-Based Modeling (ABM)** is a computational modeling approach that simulates the actions and interactions of autonomous agents in order to assess their effects on the system as a whole. ABM is especially useful for capturing non-linear dynamics, heterogeneity, and emergent phenomena.

### Core Components of ABM

- **Agents**: Autonomous actors with goals, state, and behavior.
- **Environment**: The simulated world in which agents operate. It includes space, resources, constraints, and events.
- **Interaction**: Rules that define how agents influence and are influenced by others or the environment.
- **System-level Dynamics**: Emergent outcomes from the interactions of many individual agents.

ABM is used in fields such as emergency evacuation, epidemiology, economics, urban planning, logistics, manufacturing, and biology.

## The Agent

In GRAMMA, an agent is a modular and extensible unit of simulation, designed around widely accepted ABM principles. Each agent contains the following components:

### Core Properties

- **ID**: A unique identifier.
- **Position**: A 2D (or 3D) world coordinate.
- **Type**: Optional classification label (e.g. "worker", "civilian", "machine").
- **Traits**: Static attributes defining capabilities or preferences (e.g. speed, vision range, familiarity with exits).
- **State**: The current discrete status of the agent (e.g. idle, moving, rescued, dead).

### Perception and Reasoning

- **Sensors**: Perception mechanisms that allow the agent to observe the environment or other agents (e.g. line-of-sight, proximity).
- **Memory**: Optional short- or long-term storage of previous states, decisions, or observations.
- **Rules**: Simple decision heuristics, usually local and reactive.

### Behavior

- **Needs**: High-level motivations that drive behavior (e.g. hunger, safety, task completion). Needs are evaluated continuously and prioritize actions.
- **Goals**: Specific outcomes the agent tries to achieve (e.g. reach an exit, complete a process). These are often derived from active needs.
- **Tasks**: Executable sequences that satisfy needs (e.g. move to exit, consume food, seek shelter). Tasks can be pre-defined or composed dynamically.
- **Adaptation**: Agents may exhibit adaptive behavior, such as learning from past experience, mimicking others, or adjusting strategies in real-time.

```
Need -> Goal -> Task
```

### Example

A civilian agent in an evacuation scenario may:

- Detect fire through a sensor
- Activate a `SafetyNeed`
- Evaluate visible exits via a `LineOfSightService`
- Select an exit and initiate a `SeekExitTask`
- Move toward the exit while avoiding obstacles and other agents

## Agent Interaction

Agents interact in two main ways:

### 1. **Agent-to-Agent Interaction**

- **Local avoidance**: Preventing collisions through physical or behavioral detours.
- **Social influence**: Observing and following other agents' behavior (e.g. herd behavior, imitation).
- **Communication** (optional): Exchanging messages or signals (e.g. warning, task coordination).

### 2. **Agent-to-Environment Interaction**

- **Navigation**: Moving through space while considering terrain, obstacles, or constraints.
- **Resource usage**: Consuming or competing for environmental resources (e.g. food, exits, machinery).
- **Sensing events**: Reacting to changes in the environment (e.g. alarms, hazards, changes in topology).

Interaction is mostly **local and decentralized**, leading to **emergent system-wide patterns**.

## Emergency

An **emergency** is a high-stress, time-critical, and often unpredictable situation that forces agents to act urgently. Examples include fires, evacuations, toxic spills, structural failures, or active threats.

### Why simulate emergencies?

- To evaluate safety protocols and infrastructure
- To optimize evacuation strategies
- To train personnel or test resilience
- To validate architectural designs under stress

In simulation, emergencies typically:

- Trigger specific needs (e.g. `SafetyNeed`)
- Modify environment dynamics (e.g. closing exits, adding obstacles)
- Introduce uncertainty or panic behavior

## Applications

Agent-Based Modeling is uniquely suited for scenarios where individual behavior matters and global outcomes cannot be easily derived from top-down equations. Some real-world application areas include:

- **Emergency management**: Evacuation planning, crowd control, safety analysis
- **Manufacturing systems**: Workforce simulation, resource scheduling, logistics flow
- **Urban planning**: Pedestrian movement, traffic simulation, land use modeling
- **Healthcare and epidemiology**: Disease spread, patient flow, behavior change
- **Ecology**: Predator-prey dynamics, habitat modeling
- **Economics and finance**: Market dynamics, consumer behavior, decentralized decision-making
- **Organizational modeling**: Business process simulation, team dynamics, resource allocation

Agent-Based Models offer a bottom-up approach that captures complexity, feedback loops, and heterogeneity—making them indispensable for many modern simulation needs.
