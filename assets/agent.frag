#version 330 core

in vec2 vUV;

uniform float uInnerRadius;
uniform float uOuterRadius;
uniform float uThickness;
uniform float uTime;
uniform float uBlendWidth;
uniform float uHeading;

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

		// Direction (Heading)
		vec2 dir = normalize(vUV);
    	vec2 head = vec2(sin(uHeading), cos(uHeading)); // heading vector (0 = north)
    	float d = dot(dir, head);           // cos(theta)
    	if (d > cos(radians(30.0))) {       // 15° nose
    	    FragColor = vec4(0.0, 0.0, 0.0, alpha); // small black nose
    	} else {
    	    FragColor = vec4(uColor.rgb, uColor.a * alpha);
    	}
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
