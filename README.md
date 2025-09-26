# gramma

**gramma** is a minimal C++ framework for interactive simulations and visualizations,
built to explore _"the simple grammar of the world"_ — recurring patterns across physics,
electrical engineering, biology, psychology, and AI.

The goal is **clarity and simplicity**:

- Clean C++17 architecture
- Reusable building blocks
- Real-time rendering with OpenGL (GLFW + glad + GLM)

## ✨ Features (current & planned)

- **Core infrastructure**
  - Window handling (GLFW)
  - Time & delta time
  - Basic input utilities

- **Graphics**
  - Shader wrapper (unlit / custom GLSL)
  - Camera2D (orthographic, zoom/pan)
  - Instanced rendering (particles)
  - 2D textures (grids, plots)

- **Architecture**
  - `IApp` interface (`Init`, `Update(dt)`, `Render`)
  - Clean separation: simulation ≠ rendering

- **Examples (apps)**
  - `HelloApp` (minimal triangle) ✅
  - Particle systems (attraction/repulsion) ⏳
  - Game of Life (grid texture) ⏳
  - Fourier playground (plots) ⏳
  - Crowd evacuation & social force models ⏳

## 🚀 Build & Run

### Requirements

- C++17 compiler
- CMake ≥ 3.16

### Steps

```bash
git clone https://github.com/breiting/gramma.git
cd gramma
cmake -B build
cmake --build build -j
```

Run the hello app

```bash
./build/hello
```

You should see a simple white triangle on a dark background.

## 📜 License

MIT — free to use, learn, and build upon.

## 🙏 Credits / Inspiration

- GLFW for windowing
- GLM for math
- glad for OpenGL loading
- Inspired by The Nature of Code (Shiffman) and manim (3Blue1Brown), but focused on real-time, interactive exploration in C++.
