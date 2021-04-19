#version 400

// Inputs
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniforms
uniform float time;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat4 normalMat;

// Outputs to geometry shader
out vec4 particle_color;
out float particle_id;
out float timestep;

// Constants
float grav = 0.5;
float speed = 2.5;
float upward = 1.0;
float accel = 1.2;
const float pi = 3.1415926536;
const float pi_over_two = 1.5707963268;
const float two_pi = 2.0*pi;

void main()
{
	particle_id = color.r;

	// TODO: What follows is the particle shader from COMP3009A tutorial 10, but hand-typed.
	//		 Need to replace it ASAP with our own effect, as this is just for understanding and to test the system.

	vec3 up_vec = vec3(0.0, -1.0f, 0.0);
	vec3 object_color = vec3(0.8, 0.8, 0.8);

	float phase = two_pi * particle_id;
	float param = time / 10.0 + phase;
	float rem = mod(param, pi_over_two);
	float circtime = sin(rem);
	float t = abs(circtime) * (0.3 + abs(normal.y));

	vec4 position = world * vec4(vertex, 1.0f);
	vec4 normalTwo = normalMat * vec4(normal, 1.0f);

	position.x += (normalTwo.x * t * speed) - (grav * speed * up_vec.x * t * t);
	position.y += ((upward+normalTwo.y) * t * speed) - (grav * speed * up_vec.y * t * t);
	position.z += (normalTwo.z * t * speed) - (grav * speed * up_vec.z * t * t);

	gl_Position = view * position;
	particle_color = vec4(1.0, 1, 0.0, 1);
	timestep = t;
}