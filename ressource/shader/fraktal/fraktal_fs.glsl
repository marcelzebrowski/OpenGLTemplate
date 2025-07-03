#version 330 core
out vec4 FragColor;

uniform vec2 uResolution;
uniform float uTime;
uniform vec2 uCenter;
uniform float uZoom;

// Fraktal-Iteration für Julia-Menge
int juliaIterations(vec2 z, vec2 c, int maxIterations) {
    int i;
    for (i = 0; i < maxIterations; i++) {
        if (dot(z, z) > 4.0) break;
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
    }
    return i;
}

void main() {
    vec2 uv = gl_FragCoord.xy / uResolution;
    vec2 p = (uv - 0.5) * vec2(uResolution.x / uResolution.y, 1.0);

    // Rotation für mehr Dynamik
    float angle = uTime * 0.2;
    mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    vec2 pRotated = rot * p;

    // Animiertes Zentrum
    vec2 animatedCenter = uCenter + vec2(sin(uTime * 0.1) * 0.5, cos(uTime * 0.2) * 0.5);
    vec2 c = animatedCenter + pRotated / uZoom;

    int maxIterations = 50;
    int iters = juliaIterations(pRotated, c, maxIterations);

    // Glüheffekt
    float glow = pow(float(iters) / float(maxIterations), 2.0);

    // Pulsierender Rhythmus
    float pulse = 0.9 + 0.1 * sin(uTime);

    // Warme Fraktalfarbe
    vec3 glowColor = vec3(
        0.6 + 0.4 * glow,
        0.2 * glow,
        0.1 + 0.3 * (1.0 - glow)
    );

    // Dunkler Hintergrund
    vec3 bgColor = vec3(0.05, 0.02, 0.01);

    // Mischung mit Pulsieren
    vec3 finalColor = mix(bgColor, glowColor, glow * pulse);

    FragColor = vec4(finalColor, 1.0);
}
