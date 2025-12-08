#version 330 core

in float vHeightNorm;   
out vec4 FragColor;

vec3 topoColor(int band)
{
    if (band == 0) return vec3(0.85, 0.93, 0.98);
    if (band == 1) return vec3(0.78, 0.90, 0.80);
    if (band == 2) return vec3(0.65, 0.84, 0.70);
    if (band == 3) return vec3(0.90, 0.86, 0.70);
    if (band == 4) return vec3(0.90, 0.78, 0.60);
    if (band == 5) return vec3(0.85, 0.70, 0.70);
    if (band == 6) return vec3(0.80, 0.80, 0.84);
    return            vec3(0.96, 0.96, 0.98);
}

void main()
{
    float t = clamp(vHeightNorm, 0.0, 1.0);

    // *** Treat very-low heights as "no data" and don't draw them ***
    // can be shifted a bit but like  0.02–0.05 depending on how much of the edge you want gone


// Hide only cells whose normalized height actually zero

    if (t <= 0.0001)
        discard;

    // --- quantize into 8 bands ---
    float bands  = 8.0;
    float scaled = t * bands;
    int   band   = int(floor(scaled));
    band = clamp(band, 0, 7);

    vec3 baseColor = topoColor(band);

    float bandPos = fract(scaled);
    float contour = smoothstep(0.0, 0.15, bandPos) *
                    (1.0 - smoothstep(0.85, 1.0, bandPos));
    float shade = mix(0.90, 1.05, contour);

    vec3 finalColor = baseColor * shade;
    FragColor = vec4(finalColor, 1.0);
}
