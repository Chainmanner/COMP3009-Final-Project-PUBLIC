#version 330

// Textured Phong pixel shader, with shadow mapping.
// Calculates the light at each pixel.

// Inputs
smooth in vec2 psTexCoord;
smooth in vec4 psPixelPos;
smooth in vec4 psNormal;
flat in vec4 psLightPos;

// Constants
uniform vec3 ambient_color = vec3(0.2f, 0.2f, 0.2f);
uniform vec3 diffuse_color = vec3(0.75f, 0.75f, 0.75f);
uniform vec3 specular_color = vec3(0.8f, 0.8f, 0.8f);
uniform float coeffAmb = 0.1f;
uniform float coeffDiff = 1.0f;
uniform float coeffSpec = 1.5f;
uniform float shine = 5.0f;

// Texture samplers
uniform sampler2D tex1;

void main()
{
	vec3 light1Color, light2Color;

	vec3 ambient, diffuse, specular;
	vec3 N, V, L, R;
	vec4 curLightPos;

	// ==== FIRST LIGHT ====
	curLightPos = psLightPos;
	
	// Ambient lighting
	ambient = coeffAmb * ambient_color;

	// Diffuse lighting
	N = normalize(vec3(psNormal));
	V = vec3(psPixelPos);
	L = normalize(vec3(curLightPos) - V);
	diffuse = coeffDiff * diffuse_color * max(dot(L, N), 0.0f);

	// Specular lighting
	V = normalize(-V);
	R = -L + 2 * dot(L, N) * N;
	specular = coeffSpec * specular_color * pow(max(dot(R, V), 0.0f), shine);

	// Combine the lighting
	light1Color = ambient + diffuse + specular;


	// ==== SECOND LIGHT ====
	// TODO (pending second light implementation in vertex shader)


	// Final pixel color
	gl_FragColor = texture(tex1, psTexCoord) * vec4(light1Color, 1.0f);	// TODO: Second light
}
