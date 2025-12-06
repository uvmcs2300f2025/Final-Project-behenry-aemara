#version 330 core

layout (location = 0) in vec3 aPos;   // x, y, z from your VBO

out float vHeightNorm;                // 0..1, passed to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uHeightScale;           // same value you set in Terrain::draw

void main()
{
    // Your mesh is built with y in approx [-uHeightScale/2, +uHeightScale/2]
    // So we can remap back to 0..1 here:
    float t = aPos.y / uHeightScale + 0.5;   // -0.5 → 0, +0.5 → 1
    vHeightNorm = clamp(t, 0.0, 1.0);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
