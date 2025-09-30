#version 330 core

layout(location = 0) in vec2 inPos;

uniform mat4 uMVP;
uniform vec4 uColor;

out vec4 vCol;

void main() {
  vCol = uColor;
  gl_Position = uMVP * vec4(inPos, 0.0, 1.0);
}
