#version 330 core

layout(location=0) in vec2 inPos;

uniform mat4 uMVP;

void main(){ 
	gl_Position = uMVP * vec4(inPos,0,1); 
}
