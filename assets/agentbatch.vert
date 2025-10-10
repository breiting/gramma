#version 330 core

layout (location = 0) in vec2 dummy;

// Instanced Attributes
layout (location = 1) in vec2 iPos;
layout (location = 2) in float iRadius;
layout (location = 3) in vec4 iColor;
layout (location = 4) in vec4 iGlowColor;
layout (location = 5) in float iGlowWidth;
layout (location = 6) in vec2 iHeading;

out vec2 vUV;
out float vRadius;
out vec4 vColor;
out vec4 vGlowColor;
out float vGlowWidth;
out vec2 vHeading;

uniform mat4 uMVP;

void main() {
    vec2 corner = vec2(
        (gl_VertexID == 1 || gl_VertexID == 3) ? 1.0 : -1.0,
        (gl_VertexID >= 2) ? 1.0 : -1.0
    );

    vUV          = corner;
    vRadius      = iRadius;
    vColor       = iColor;
    vGlowColor   = iGlowColor;
    vHeading     = iHeading;
	vGlowWidth   = iGlowWidth;

    vec2 worldPos = iPos + corner * iRadius;
    gl_Position = uMVP * vec4(worldPos, 0.0, 1.0);
}
