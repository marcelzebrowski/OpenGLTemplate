#version 330 core
out vec4 FragColor;

uniform vec2 uResolution;
uniform float uTime;
uniform vec2 uCenter;
uniform float uZoom;

// Funktion, die die Iterationen des Mandelbrot-Fraktals berechnet
int mandelbrotIterations(vec2 c, int maxIterations) {
    vec2 z = vec2(0.0);
    int i;
    for (i = 0; i < maxIterations; i++) {
        if (dot(z, z) > 4.0) break; // Wenn der Betrag von z größer als 2 ist
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
    }
    return i;
}

int juliaIterations(vec2 z, vec2 c, int maxIterations) {
    int i;
    for (i = 0; i < maxIterations; i++) {
        if (dot(z, z) > 4.0) break; // Wenn der Betrag von z größer als 2 ist
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
    }
    return i;
}

int burningShipIterations(vec2 c, int maxIterations) {
    vec2 z = vec2(0.0);
    int i;
    for (i = 0; i < maxIterations; i++) {
        if (dot(z, z) > 4.0) break; // Wenn der Betrag von z größer als 2 ist
        z = vec2(abs(z.x) * abs(z.x) - abs(z.y) * abs(z.y), 2.0 * abs(z.x) * abs(z.y)) + c;
    }
    return i;
}


void main() {
    // Pixel-Koordinaten in den Bereich [0, 1] normalisieren
    vec2 uv = gl_FragCoord.xy / uResolution;

    // Normalisierte Koordinaten in den Bereich von -1 bis 1 skalieren, basierend auf der Auflösung
    vec2 p = (uv - 0.5) * 2.0 * vec2(uResolution.x / uResolution.y, 1.0);

    // Berechnung eines animierten Zentrumspunkts (`uCenter`), das sich im Verlauf der Zeit verändert
    vec2 animatedCenter = uCenter + vec2(sin(uTime * 0.1) * 0.5, cos(uTime * 0.2) * 0.5); // Dynamische Bewegung

    // Zoomen und Zentrieren unter Verwendung des angepassten Zentrumspunkts
    vec2 c = animatedCenter + p / uZoom;
    vec2 c1 = p / uZoom;

    // Fraktal-Iterationen berechnen
    int maxIterations = 300;
    int iters = juliaIterations(p,c, maxIterations);

    // Glatte Farbinterpolation
    float color = float(iters) / float(maxIterations);

    // Farbzuweisung basierend auf den Iterationen
    vec3 col = vec3(color * 0.5, sin(color * 3.1415), color);

    FragColor = vec4(col, 1.0);
}