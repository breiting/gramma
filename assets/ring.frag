#version 330 core

in vec2 vPos;
in vec4 vCol;

out vec4 FragColor;

uniform float uRadius;  
uniform float uThickness;
uniform float uGlow;    
uniform float uTime;   

void main() {
    float dist = length(vPos);

    float inner = uRadius - 0.5 * uThickness;
    float outer = uRadius + 0.5 * uThickness;

    float ring = smoothstep(inner - uGlow, inner, dist) * 
                 (1.0 - smoothstep(outer, outer + uGlow, dist));

    // Pulsation 
    float pulse = 0.75 + 0.25 * sin(uTime * 2.0);

    FragColor = vec4(vCol.rgb, vCol.a * ring * pulse);
}
