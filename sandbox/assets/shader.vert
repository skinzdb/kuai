#version 450

layout (location = 0)	in vec3 aPos;
layout (location = 1)	in vec3 aNormal;
layout (location = 2)	in vec2 aTexCoord;
// layout (location = 3)	in mat4 aModelMatrix;

// layout (binding = 0) uniform CamData
// {
// 	mat4 projectionMatrix;
// 	mat4 viewMatrix;
// 	vec3 viewPos;
// };

layout (location = 0) out vec4 world_pos;
layout (location = 1) out vec3 world_norm;
layout (location = 2) out vec2 tex_coords;
// layout (location = 3) out vec3 viewing_pos;

void main()
{
	world_pos = vec4(aPos, 1.0);
	// mat3 model3x3InvTransp = mat3(transpose(inverse(aModelMatrix))); // TODO: remove in favour of vertex attribute
	// world_norm = model3x3InvTransp * aNormal;
	world_norm = aNormal;
	tex_coords = aTexCoord;
	// viewing_pos = viewPos;

	gl_Position = world_pos;
}
