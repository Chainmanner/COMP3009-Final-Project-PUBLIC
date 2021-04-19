#version 330

// Textured Phong vertex shader.
// Calculates the normal at a vertex, which is to be interpolated for each pixel.

// Inputs
in vec3 vertex;
in vec3 normal;
in vec2 vsTexCoord;

// Outputs
smooth out vec2 psTexCoord;
smooth out vec4 psPixelPos;
smooth out vec4 psNormal;
flat out vec4 psLightPos;

// Uniforms
uniform vec3 lightPos;

// Matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

void main()
{
	// Pass the texture coordinate to the pixel shader.
	psTexCoord = vsTexCoord;

	// Calculate the view-space vertex coordinates.
	psPixelPos = view * world * vec4(vertex, 1.0f);

	// Calculate the vertex position.
	gl_Position = projection * psPixelPos;

	// Convert the light position to view coordinates, then output it.
	psLightPos = view * vec4(lightPos, 1.0f);

	// Convert the normal to view coordinates, then output it.
	psNormal = transpose(inverse(view * world)) * vec4(normal, 1.0f);
}
