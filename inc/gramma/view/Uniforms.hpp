#pragma once

namespace gr::Uniforms {

// Transformation
inline constexpr const char* MVP = "uMVP";  // mat4: Model-View-Projection

// Material / Color
inline constexpr const char* COLOR = "uColor";             // vec4: RGBA
inline constexpr const char* TEXTURE = "uTexture";         // sampler2D (for textured quads, text)
inline constexpr const char* USE_TEXTURE = "uUseTexture";  // texture available?

// Time (for animations, pulsation etc.)
inline constexpr const char* TIME = "uTime";  // float: elapsed time in seconds

// Special (optional)
inline constexpr const char* RADIUS = "uRadius";        // float: used by circle shaders
inline constexpr const char* THICKNESS = "uThickness";  // float: line thickness / outline width

}  // namespace gr::Uniforms
