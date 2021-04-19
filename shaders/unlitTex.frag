#version 330

// Shader for unlit and textured objects. Mainly intended to be used as a placeholder.

// Inputs
in vec2 psTexCoord;

// Outputs
out vec4 psColor;

// Texture samplers
uniform sampler2D tex1;

void main(void)
{
	psColor = texture(tex1, psTexCoord);
}
