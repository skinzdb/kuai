#version 450

layout (location = 0) in vec4 world_pos;
layout (location = 1) in vec3 world_norm;
layout (location = 2) in vec2 tex_coords;

layout (location = 0) out vec4 fragCol;

void main()
{
    fragCol = vec4(world_norm, 1.0);
}
