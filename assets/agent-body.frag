#version 330 core

in vec2 vUV;
in float vRadius;
in vec4 vColor;
in vec4 vGlowColor;
in float vGlowWidth;
in vec2 vHeading;

out vec4 FragColor;

void main() {
    float dist = length(vUV) * vRadius;


    float glowStart = vRadius - vGlowWidth;
    float alpha = 1.0;

    if (dist <= glowStart) {
        // Volle Farbe im inneren Kreis
        FragColor = vec4(vColor.rgb, vColor.a);
        return;
    }

    if (dist <= vRadius) {
        // Weicher Übergang zum Glow
        float t = smoothstep(glowStart, vRadius, dist);
        vec3 blendedColor = mix(vColor.rgb, vGlowColor.rgb, t);
        float blendedAlpha = mix(vColor.a, vGlowColor.a, t);
        FragColor = vec4(blendedColor, blendedAlpha);
        return;
    }

    discard;
}
