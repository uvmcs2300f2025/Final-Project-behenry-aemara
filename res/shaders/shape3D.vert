#version 330 core

layout (location = 0) in vec3 aPos;   

out float vHeightNorm;                

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uHeightScale;           

void main()
{

    float t = aPos.y / uHeightScale + 0.5;   // -0.5 → 0, +0.5 → 1
    vHeightNorm = clamp(t, 0.0, 1.0);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
