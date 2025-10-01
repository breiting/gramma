#version 330 core

in vec2 vUV;

uniform float uInnerRadius;
uniform float uOuterRadius;
uniform float uThickness;
uniform float uTime;
uniform float uBlendWidth;

uniform vec4  uColor;
uniform vec4  uGlowColor;

out vec4 FragColor;

void main() {

    float dist = length(vUV);

    // Body
    if (dist < uInnerRadius) {
        float alpha = 1.0;
        if (dist > uInnerRadius - uBlendWidth) {
            alpha = smoothstep(uInnerRadius, uInnerRadius - uBlendWidth, dist);
        }
        FragColor = vec4(uColor.rgb, uColor.a * alpha);
        return;
    }

    // Glow
    if (dist > uOuterRadius - uThickness && dist < uOuterRadius) {
        float glow = smoothstep(uOuterRadius, uOuterRadius - uThickness, dist);
        float pulse = 0.5 + 0.5 * sin(2.0 * 3.14159 * 0.5 /*Hz*/ * uTime);
        FragColor = vec4(uGlowColor.rgb, uGlowColor.a * glow * 0.8 * pulse);
        return;
    }

	discard;
}
