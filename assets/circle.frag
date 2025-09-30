#version 330 core

in vec2 vLocal;
in vec4 vColor;

out vec4 fragColor;

uniform float uAlpha;

void main(){
  float r = length(vLocal);                 // quad is [-1,1] → radius = 1
  float a = smoothstep(1.0, 0.9, r);        // soft edge
  fragColor = vec4(vColor.rgb, vColor.a * a * uAlpha);
}
