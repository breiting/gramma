#version 330 core

layout (location = 0) in vec2 dummy;

// Instanced Attributes
layout (location = 1) in vec2 iPos;
layout (location = 2) in float iInnerRadius;
layout (location = 3) in float iOuterRadius;
layout (location = 4) in float iBlendWidth;
layout (location = 5) in vec4 iColor;
layout (location = 6) in vec4 iGlowColor;
layout (location = 7) in float iThickness;
layout (location = 8) in float iHeading;
layout (location = 9) in float iFOV;

out vec2 vUV;
out float vInnerRadius;
out float vOuterRadius;
out float vBlendWidth;
out vec4 vColor;
out vec4 vGlowColor;
out float vThickness;
out float vHeading;
out float vFOV;

uniform mat4 uMVP;

void main() {
    vec2 corner = vec2(
        (gl_VertexID == 1 || gl_VertexID == 3) ? 1.0 : -1.0,
        (gl_VertexID >= 2) ? 1.0 : -1.0
    );

    vUV          = corner;
    vInnerRadius = iInnerRadius;
    vOuterRadius = iOuterRadius;
    vBlendWidth  = iBlendWidth;
    vColor       = iColor;
    vGlowColor   = iGlowColor;
    vThickness   = iThickness;
    vHeading     = iHeading;
    vFOV         = iFOV;

    vec2 worldPos = iPos + corner * iOuterRadius;
    gl_Position = uMVP * vec4(worldPos, 0.0, 1.0);
}
