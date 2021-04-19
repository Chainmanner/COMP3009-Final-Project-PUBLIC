#version 400

// Copied from COMP3009A tutorial 10. No credit is taken by the group members for this file.

// Definition of the geometry shader
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// Attributes passed from the vertex shader
in vec4 particle_color[];
in float timestep[];
in float particle_id[];

// Uniform (global) buffer
uniform mat4 projection_mat;

// Simulation parameters (constants)
float particle_size = 0.02;

// Attributes passed to the fragment shader
out vec4 frag_color;
// add any other attributes you want here

void main(void){

    // Get the position of the particle
    vec4 position = gl_in[0].gl_Position;

    // Define particle size
    float p_size = particle_size;

    // Define the positions of the four vertices that will form a quad 
    // The positions are based on the position of the particle and its size
    // We simply add offsets to the position (we can think of it as the center of the particle),
    // since we are already in camera space
    vec4 v[4];
    v[0] = vec4(position.x - 0.5*p_size, position.y - 0.5*p_size, position.z, 1.0);
    v[1] = vec4(position.x + 0.5*p_size, position.y - 0.5*p_size, position.z, 1.0);
    v[2] = vec4(position.x - 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);
    v[3] = vec4(position.x + 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);

    // Create the new geometry: a quad with four vertices from the vector v
    for (int i = 0; i < 4; i++){
        gl_Position = projection_mat * v[i];
        frag_color = vec4(particle_color[0]);
        EmitVertex();
     }

     EndPrimitive();
}
