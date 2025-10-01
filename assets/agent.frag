#version 330 core

in vec2 vUV;

uniform float uInnerRadius;
uniform float uOuterRadius;
uniform vec4  uColor;
uniform float uTime;

out vec4 FragColor;

void main() {

    float dist = length(vUV);

    // Body
	float edge = 1.0;
	float bodyAlpha = smoothstep(uInnerRadius, uInnerRadius - edge, dist);
	vec4 body = vec4(uColor.rgb, uColor.a * bodyAlpha);

    // Glow
    vec4 glow = vec4(0.0);
    if (dist < uOuterRadius) {
        float glowFactor = 1.0 - (dist - uInnerRadius) / (uOuterRadius - uInnerRadius);
        float pulse = 0.5 + 0.5 * sin(2.0 * 3.14159 * 1.0 /*Hz*/ * uTime); // 1 Hz
        glow = vec4(uColor.rgb, uColor.a * glowFactor * 0.6 * pulse);
    }

    FragColor = body + glow;

    if (FragColor.a < 0.01)
        discard;
}
