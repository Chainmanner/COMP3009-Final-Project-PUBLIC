#version 330 core

// Inputs
in vec3 vertex;
in vec3 normal;		// Unneeded.
in vec3 texCoord;	// Unneeded.

// Matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

void main()
{
	gl_Position = projection * view * world * vec4(vertex, 1.0f);
}
