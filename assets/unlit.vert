#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex;
layout(location = 2) in vec4 inCol;

uniform mat4 uMVP;
out vec2 vTex;
out vec4 vCol;

void main() {
  vTex = inTex;
  vCol = inCol;
  gl_Position = uMVP * vec4(inPos, 0.0, 1.0);
}
