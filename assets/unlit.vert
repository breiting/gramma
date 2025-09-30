#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;

uniform mat4 uMVP;
out vec4 vCol;

void main() {
  vCol = inCol;
  gl_Position = uMVP * vec4(inPos, 0.0, 1.0);
}
