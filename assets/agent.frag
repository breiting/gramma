#version 330 core

in vec2 vUV;

uniform float uInnerRadius;
uniform float uOuterRadius;
uniform float uThickness;
uniform float uTime;
uniform float uBlendWidth;
uniform float uHeading;
uniform float uFOV;

uniform vec4  uColor;
uniform vec4  uGlowColor;

out vec4 FragColor;

void main() {

    float dist = length(vUV);

    // Body with heading (FOV)
    if (dist < uInnerRadius) {
        float alpha = 1.0;
        if (dist > uInnerRadius - uBlendWidth) {
            alpha = smoothstep(uInnerRadius, uInnerRadius - uBlendWidth, dist);
        }

        vec2 dir = normalize(vUV);
        vec2 head = vec2(sin(uHeading), cos(uHeading));

        float d = dot(dir, head);
        float ang = acos(clamp(d, -1.0, 1.0));

        float fovRad = radians(uFOV);
        float fovEdge = radians(3.0); // 3° smoothing
        float mask = 1.0 - smoothstep(fovRad - fovEdge, fovRad, ang);

        vec3 baseCol = uColor.rgb;
        vec3 col = mix(baseCol, uGlowColor.rgb, mask);

        FragColor = vec4(col, uColor.a * alpha);
        return;
    }

    // Glow
    float inner = uOuterRadius - uThickness;
    float outer = uOuterRadius;

    if (dist >= inner && dist <= outer) {
        // Smooth inner border
        float maskIn  = smoothstep(inner, inner + uBlendWidth, dist);
        // Smooth outer border
        float maskOut = smoothstep(outer, outer - uBlendWidth, dist);

        float ringAlpha = maskIn * maskOut;

        // Pulsation
        float pulse = 0.5 + 0.5 * sin(2.0 * 3.14159 * 0.5 /*Hz*/ * uTime);

        FragColor = vec4(uGlowColor.rgb, uGlowColor.a * ringAlpha * pulse);
        return;
    }

    discard;
}
