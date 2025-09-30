#version 330 core

layout(location=0) in vec2 inLocal;       // quad corners in [-1,1]
layout(location=1) in vec2 inPos;         // world position (m)
layout(location=2) in float inSize;       // diameter (m)
layout(location=3) in vec4 inColor;       // color

uniform mat4 uMVP;

out vec2 vLocal;
out vec4 vColor;

void main(){
  vec2 world = inPos + 0.5 * inSize * inLocal;
  gl_Position = uMVP * vec4(world, 0.0, 1.0);
  vLocal = inLocal;
  vColor = inColor;
}
