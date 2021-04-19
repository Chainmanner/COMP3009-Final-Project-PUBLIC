#version 400

// Copied from COMP3009A tutorial 10. No credit is taken by the group members for this file.

// Attributes passed from the geometry shader
in vec4 frag_color;

// Simulation parameters (constants)
uniform vec3 object_color = vec3(0.8, 0.4, 0.03);

out vec4 fragColor;

void main (void)
{
    // Get pixel from texture
    vec4 outval = frag_color;
    // Set output fragment color
 
	//IF ON WINDOWS USE gl_FragColor
	gl_FragColor = outval;

	//IF ON MAC USE fragColor
	//fragColor = outval;
}
