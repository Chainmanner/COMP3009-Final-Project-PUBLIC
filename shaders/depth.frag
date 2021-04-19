#version 330 core

// No other inputs.

void main()
{
	// This line is explicit, but not really needed.
	gl_FragDepth = gl_FragCoord.z;
	gl_FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0f);
}
