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

    float innerRadius = vRadius - vGlowWidth;
    float alpha = 0.0;

    // --- Body with glow ---
    if (dist < vRadius) {
        alpha = smoothstep(vRadius, innerRadius, dist);

        vec3 color = vColor.rgb;

        // Highlight heading direction (thin line)
        vec2 dir = normalize(vUV);
        float alignment = dot(dir, vHeading);
        float angleHighlight = smoothstep(0.98, 1.0, alignment);  // thin forward line

        color += vec3(1.0) * angleHighlight * 0.4;

        FragColor = vec4(color, alpha * vColor.a);
        return;
    }
}
