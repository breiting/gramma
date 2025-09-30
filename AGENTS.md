# Agent Instructions for gramma

## Build Commands

- **Build all**: `cmake -B build && cmake --build build -j`
- **Build hello app**: `cmake -B build && cmake --build build -j --target hello`
- **Build bounce app**: `cmake -B build && cmake --build build -j --target bounce`
- **Run hello app**: `./build/hello`
- **Run bounce app**: `./build/bounce`

## Code Style Guidelines

- **Language**: C++17 with CMake build system
- **Formatting**: clang-format with Google style (4-space indent, 120 column limit)
- **Includes**: System headers first, then local headers with quotes
- **Namespaces**: Use `gr` namespace for all code
- **Naming**: PascalCase for classes/functions, camelCase for variables, `m_` prefix for member variables in combination with PascalCase `m_PrivateMember`
- **Error handling**: Use `std::runtime_error` for exceptions, `std::cerr` for logging
- **Headers**: Use `#pragma once`, minimal includes in headers
- **Types**: Use GLM for math types, explicit types over auto where clarity matters
