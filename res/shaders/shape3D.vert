#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
    // Directly use the vertex position as clip-space coordinates
    gl_Position = vec4(aPos, 1.0);
}
