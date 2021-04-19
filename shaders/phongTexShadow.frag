#version 330

// Textured Phong pixel shader, with shadow mapping.
// Calculates the light at each pixel.

// Inputs
smooth in vec2 psTexCoord;
smooth in vec4 psPixelPos;
smooth in vec4 psNormal;
flat in vec4 psLightPos;
smooth in vec4 psPixelPos_lightSpace;	// Needed for shadow mapping.

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
uniform sampler2D depthMap;	// Needed for shadow mapping.

void main()
{
	vec3 lightColor;

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

	// ==== SHADOW MAPPING ====
	
	// Convert the light-space pixel coordinates from homogeneous to Cartesian.
	// Also adjust the coordinates to be in the [0,1] range, so that we can use them to access the depth map.
	float perspective = psPixelPos_lightSpace.w;
	vec3 projCoords = vec3((psPixelPos_lightSpace.x / perspective) * 0.5 + 0.5,
							(psPixelPos_lightSpace.y / perspective) * 0.5 + 0.5,
							(psPixelPos_lightSpace.z / perspective) * 0.5 + 0.5);

	// Get the depth of the corresponding pixel in the depth map.
	float closestDepth = texture(depthMap, vec2(projCoords.x, projCoords.y)).r;

	// Have a small bias to prevent shadow acne (dark lines in the unshadowed, lit parts).
	// There may still be some present, though, especially when seeing the lit object at an angle.
	// The bias is calculated based on the angle between the pixel normal and the light source.
	float bias = max(0.005 * tan(acos(dot(N,L))), 0.005);
	//bias = clamp(bias, 0, 0.1);
	
	// This pixel is in the shadow if the light-space coordinate is further than the same pixel in the depth map.
	// If it's in a shadow, then it gets no light.
	float notInShadow = projCoords.z - bias < closestDepth ? 1.0f : 0.0f;

	// Combine the different light sources.
	lightColor = ambient + notInShadow * (diffuse + specular);

	// Final pixel color
	gl_FragColor = texture(tex1, psTexCoord) * vec4(lightColor, 1.0f);
}
