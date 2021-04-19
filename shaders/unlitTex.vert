#version 330

// Shader for unlit and textured objects. Mainly intended to be used as a placeholder.

// Inputs
in vec3 vertex;
in vec3 normal;
in vec2 vsTexCoord;

// Outputs
out vec2 psTexCoord;

// Uniform variables
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

void main(void)
{
	gl_Position = projection * view * world * vec4(vertex, 1.0f);
	psTexCoord = vsTexCoord;
}
