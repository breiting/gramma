#version 330 core

layout (location = 1) in vec2 iPos;
layout (location = 2) in float iRadius;
layout (location = 3) in vec4 iColor;
layout (location = 4) in vec4 iGlowColor;
layout (location = 5) in float iGlowWidth;
layout (location = 6) in vec2 iHeading;

uniform mat4 uMVP;

void main() {
    float t = (gl_VertexID == 0) ? 0.0 : 1.5;
    vec2 offset = normalize(iHeading) * iRadius * t;
    vec2 worldPos = iPos + offset;
    gl_Position = uMVP * vec4(worldPos, 0.0, 1.0);
}
