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

    float alpha = smoothstep(vRadius, innerRadius, dist);

    FragColor = vec4(vColor.rgb, alpha * vColor.a);
}
