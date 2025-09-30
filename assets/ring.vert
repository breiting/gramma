#version 330 core
layout(location = 0) in vec2 inPos;

uniform mat4 uMVP;
uniform float uRadius;
uniform vec4 uColor;

out vec2 vPos;
out vec4 vCol;

void main() {
	vCol = uColor;
    vPos = inPos * uRadius;
    gl_Position = uMVP * vec4(vPos, 0.0, 1.0);
}
