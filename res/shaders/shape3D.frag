#version 330 core

in float vHeightNorm;   // 0..1 normalized height
out vec4 FragColor;

void main()
{
    float t = clamp(vHeightNorm, 0.0, 1.0);

    // --- Hide nodata / padding: treat very low heights as background ---
    // This should match your glClearColor(0.0, 0.0, 0.2, 1.0);
    if (t < 0.02)
    {
        FragColor = vec4(0.0, 0.0, 0.2, 1.0);
        return;
    }

    // --- Topographic color ramp: green -> tan -> white ---

    // low elevations: dark-ish green
    vec3 low  = vec3(0.09, 0.25, 0.12);
    // mid-low: lighter green
    vec3 mid1 = vec3(0.28, 0.45, 0.20);
    // mid-high: tan / rock
    vec3 mid2 = vec3(0.65, 0.54, 0.32);
    // high elevations: near-white
    vec3 high = vec3(0.96, 0.96, 0.96);

    vec3 baseColor;
    if (t < 0.3)
    {
        float u = smoothstep(0.0, 0.3, t);
        baseColor = mix(low, mid1, u);
    }
    else if (t < 0.6)
    {
        float u = smoothstep(0.3, 0.6, t);
        baseColor = mix(mid1, mid2, u);
    }
    else
    {
        float u = smoothstep(0.6, 1.0, t);
        baseColor = mix(mid2, high, u);
    }

    // --- Subtle contour bands for a topo-map feel ---
    // Adjust 40.0 to change band density
    float bands = fract(t * 40.0);
    float line  = smoothstep(0.0, 0.03, min(bands, 1.0 - bands));
    // line ≈ 0 near band centers, ≈1 near edges

    vec3 contourColor = baseColor * 0.6;  // slightly darker
    vec3 finalColor   = mix(contourColor, baseColor, line);

    FragColor = vec4(finalColor, 1.0);
}
