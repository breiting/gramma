#version 330 core
layout(location=0) in vec2 inPos;
uniform mat4 uVP;
void main(){ gl_Position = uVP * vec4(inPos,0,1); }