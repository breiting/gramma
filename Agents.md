# Repository Guidelines

## Project Structure & Module Organization
Source lives under `src/` with `core` covering runtime services (runner, window, timing), `model` hosting agents, needs, tasks, and physics, `view` rendering meshes and shaders, and `ui` wrapping Dear ImGui layers. Public headers mirror this layout in `inc/gramma`. Scenario apps reside in `apps/` (`sim`, `rescue`, `particle`, `play`) and link against the shared `gramma` library. Assets such as GLSL shaders live in `assets/`; the CMake embed step mirrors them into `build/generated/assets`. External dependencies are vendored in `extern/`. Keep transient build products in `build/`.

## Build, Test, and Development Commands
- `cmake -B build -DCMAKE_BUILD_TYPE=Debug`: generate a local toolchain with reload-safe flags.
- `cmake --build build -j`: compile the core library plus all enabled apps.
- `cmake --build build --target sim` (replace target): rebuild a single app quickly.
- `./build/sim` or `./build/rescue`: execute interactive demos from the build tree.

## Coding Style & Naming Conventions
Formatting is enforced by `.clang-format` (Google base, 4-space indent, 120 column limit, no single-line functions). Prefer `PascalCase` for classes and methods (`Agent::EvaluateNeeds`), `snake_case` for free utilities, and prefix member fields with `m_`. Keep headers self-contained and include the matching header first. Organize namespaces under the short `gr` alias used across `src/model`.

## Testing Guidelines
Automated tests are not yet present; when adding them prefer lightweight fixtures colocated in `apps/<scenario>/tests` or a new `tests/` target registered via CMake with `add_test`. Validate behavior by running the relevant app with representative scenarios and document any manual steps in the PR. Aim for coverage of new agent behaviors (needs, tasks, sensors) via deterministic inputs to avoid flaky physics.

## Commit & Pull Request Guidelines
Follow the existing short, imperative commit style (`integrate TBB`, `smooth camera`). Group related changes and avoid formatting-only commits. Every PR should describe the scenario touched, list build/run steps, and attach screenshots or GIFs when UI changes are visible. Link tracking issues where applicable and note any follow-up work required.

## Agent Implementation Tips
Implement new agent capabilities inside `src/model` and register them through `TaskFactory` so apps can compose them. Lean on existing needs (`EnergyNeed`, `SafetyNeedTaskBuilder`) instead of duplicating logic, and wire new sensors through `LineOfSightService` or `VisionSensor` to stay compatible with the simulation loop.
