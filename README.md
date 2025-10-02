# gramma

**gramma** is a minimal C++ framework for interactive simulations and visualizations,
built to explore _"the simple grammar of the world"_ — recurring patterns across physics,
electrical engineering, biology, psychology, and AI.

The goal is **clarity and simplicity**:

- Clean C++17 architecture
- Reusable building blocks
- Real-time rendering with OpenGL (GLFW + glad + GLM)

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

## 📜 License

MIT — free to use, learn, and build upon.

## 🙏 Credits / Inspiration

- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)
- Inspired by The Nature of Code (Shiffman) and manim (3Blue1Brown), but focused on real-time, interactive exploration in C++.

### Additional

- `gramma` uses the [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))
