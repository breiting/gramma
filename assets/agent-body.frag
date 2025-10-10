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

    float solidZone      = vRadius - 2.0 * vGlowWidth;
    float fadeInStart    = solidZone;
    float fadeInEnd      = vRadius - vGlowWidth;
    float fadeOutStart   = fadeInEnd;
    float fadeOutEnd     = vRadius;

    if (dist <= solidZone) {
        // Full solid color inside
        FragColor = vec4(vColor.rgb, vColor.a);
        return;
    }

    if (dist <= fadeInEnd) {
        // Fade from body color to glow color
        float t = smoothstep(fadeInStart, fadeInEnd, dist);
        vec3 blendedColor = mix(vColor.rgb, vGlowColor.rgb, t);
        float blendedAlpha = mix(vColor.a, vGlowColor.a, t);
        FragColor = vec4(blendedColor, blendedAlpha);
        return;
    }

    if (dist <= fadeOutEnd) {
        // Fade out glow to transparent
        float t = smoothstep(fadeOutStart, fadeOutEnd, dist);
        vec3 blendedColor = vGlowColor.rgb;
        float blendedAlpha = mix(vGlowColor.a, 0.0, t);
        FragColor = vec4(blendedColor, blendedAlpha);
        return;
    }

    discard;
}
