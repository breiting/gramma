#version 330 core

in vec2 vUV;

uniform float uInnerRadius;
uniform float uOuterRadius;
uniform vec4  uColor;
uniform float uTime;

out vec4 FragColor;

void main() {
    float dist = length(vUV) * uOuterRadius;

    // Body
    if (dist < uInnerRadius) {
        FragColor = uColor;
        return;
    }

    // Glow
    if (dist < uOuterRadius) {
        float glow = 1.0 - (dist - uInnerRadius) / (uOuterRadius - uInnerRadius);
        float pulse = 0.5 + 0.5 * sin(2.0 * 3.14159 * 2.0 * uTime); // 2 Hz
        FragColor = vec4(uColor.rgb, uColor.a * glow * 0.6 * pulse);
        return;
    }

    discard; // outside invisible
}
