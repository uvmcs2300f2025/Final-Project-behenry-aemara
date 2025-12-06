#version 330 core

in float vHeightNorm;
out vec4 FragColor;

void main()
{
    float t = clamp(vHeightNorm, 0.0, 1.0);

    // OPTIONAL: treat *very* low normalized heights as “background”
    // This helps hide the big flat outside-of-VT slab.
    if (t < 0.01)
    {
        discard;  // let the clear color (navy) show instead
    }

    // simple 3-color gradient: low = green, mid = brown, high = white
    vec3 lowColor  = vec3(0.10, 0.35, 0.10);  // low elevations (valleys, foothills)
    vec3 midColor  = vec3(0.60, 0.45, 0.25);  // mid elevations (hills)
    vec3 highColor = vec3(1.00, 1.00, 1.00);  // high elevations (ridges, peaks)

    vec3 color;
    if (t < 0.5)
    {
        float k = t * 2.0;
        color = mix(lowColor, midColor, k);
    }
    else
    {
        float k = (t - 0.5) * 2.0;
        color = mix(midColor, highColor, k);
    }

    FragColor = vec4(color, 1.0);
}
