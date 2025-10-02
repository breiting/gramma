#version 330 core

in vec2 vUV;
in float vInnerRadius;
in float vOuterRadius;
in float vBlendWidth;
in vec4 vColor;
in vec4 vGlowColor;
in float vThickness;
in float vHeading;
in float vFOV;

out vec4 FragColor;

void main() {
    float dist = length(vUV) * vOuterRadius;

    // --- Body ---
    if (dist < vInnerRadius) {
        float alpha = 1.0;
        if (dist > vInnerRadius - vBlendWidth) {
            alpha = smoothstep(vInnerRadius, vInnerRadius - vBlendWidth, dist);
        }

        // Heading
        vec2 dir = normalize(vUV);
        vec2 head = vec2(sin(vHeading), cos(vHeading)); // Heading (0 = Norden)
        float d = dot(dir, head);

        if (d > cos(radians(vFOV))) {
            FragColor = vec4(vColor.rgb * 1.2, 1.0); // heller im FOV
        } else {
            FragColor = vec4(vColor.rgb, vColor.a * alpha);
        }
        return;
    }

    // --- Glow-Ring ---
    float inner = vOuterRadius - vThickness;
    float outer = vOuterRadius;

    if (dist >= inner && dist <= outer) {
        float maskIn  = smoothstep(inner, inner + vBlendWidth, dist);
        float maskOut = smoothstep(outer, outer - vBlendWidth, dist);
        float ringAlpha = maskIn * maskOut;


        FragColor = vec4(vGlowColor.rgb, vGlowColor.a * ringAlpha);
        return;
    }

    discard;
}
