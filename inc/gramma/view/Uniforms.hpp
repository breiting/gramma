#pragma once

namespace gr::Uniforms {

// Transformation
inline constexpr const char* MVP = "uMVP";  // mat4: Model-View-Projection

// Material / Color
inline constexpr const char* COLOR = "uColor";             // vec4: RGBA
inline constexpr const char* GLOW_COLOR = "uGlowColor";    // vec4: RGBA
inline constexpr const char* TEXTURE = "uTexture";         // sampler2D (for textured quads, text)
inline constexpr const char* USE_TEXTURE = "uUseTexture";  // texture available?

// Time (for animations, pulsation etc.)
inline constexpr const char* TIME = "uTime";  // float: elapsed time in seconds

// Special (optional)
inline constexpr const char* POSITION = "uPos";              // vec2: position
inline constexpr const char* INNER_RADIUS = "uInnerRadius";  // float: used by agent shaders
inline constexpr const char* OUTER_RADIUS = "uOuterRadius";  // float: used by agent shaders
inline constexpr const char* THICKNESS = "uThickness";       // float: line thickness / outline width
inline constexpr const char* BLEND_WIDTH = "uBlendWidth";    // float: Anti-alias Blending width (world-coordinates)
inline constexpr const char* HEADING = "uHeading";           // float: Heading/Orientation information (radians)
inline constexpr const char* FOV = "uFOV";                   // float: Field Of View

}  // namespace gr::Uniforms
