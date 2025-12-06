#version 330 core

in float vHeightNorm;   // 0..1 normalized height from vertex shader
out vec4 FragColor;

void main()
{
    float t = clamp(vHeightNorm, 0.0, 1.0);

    // --- 0. Hide nodata / padding (very low heights) ---
    vec3 bgColor = vec3(0.0, 0.0, 0.2);  // must match glClearColor
    if (t < 0.015)
    {
        FragColor = vec4(bgColor, 1.0);
        return;
    }

    // --- 1. Quantize into N discrete elevation bands ---
    const int NUM_BANDS = 8;

    // Height band index: 0 .. NUM_BANDS-1
    int bandIndex = int(floor(t * float(NUM_BANDS)));
    if (bandIndex >= NUM_BANDS)
        bandIndex = NUM_BANDS - 1;

    // --- 2. Define a cute topo palette (low → high) ---

    // Topographic palette: valley greens → hills → rock/tan → snowy peaks
    const vec3 topoColors[NUM_BANDS] = vec3[](
        vec3(0.76, 0.93, 0.80),  // 0: soft mint green (lowest valleys)
        vec3(0.61, 0.85, 0.66),  // 1: light grassy green
        vec3(0.47, 0.74, 0.55),  // 2: medium green
        vec3(0.80, 0.86, 0.60),  // 3: yellow-green transition
        vec3(0.89, 0.83, 0.60),  // 4: warm tan
        vec3(0.80, 0.72, 0.56),  // 5: light brown / rock
        vec3(0.85, 0.85, 0.85),  // 6: light gray / high rock
        vec3(0.98, 0.97, 0.98)   // 7: almost white snowcaps
    );

    vec3 baseColor = topoColors[bandIndex];

    // --- 3. OPTIONAL: subtle band edges to emphasize "layers" ---
    // This just darkens right at the border between bands.
    float bandPos = t * float(NUM_BANDS);    // 0..NUM_BANDS
    float fracPos = fract(bandPos);          // position within current band [0,1]
    float edge = min(fracPos, 1.0 - fracPos);

    // lineStrength ≈1 near band edges, ≈0 in middle of band.
    float lineStrength = 1.0 - smoothstep(0.0, 0.08, edge);

    vec3 edgeColor = baseColor * 0.75;       // slightly darker at edges
    vec3 finalColor = mix(baseColor, edgeColor, lineStrength * 0.6);

    FragColor = vec4(finalColor, 1.0);
}
