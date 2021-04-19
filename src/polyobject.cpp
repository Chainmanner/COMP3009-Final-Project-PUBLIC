#include <constants.h>
#include <polyobject.h>
#include <utils.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>


// Creates a cube.
// FIXME: Are the faces culled right now?
PolyObject* CreateCube(std::string textureName, PolyObject* parent, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	// Array contains each vertex's position, followed by its texture coordinate.
	float temp_vertices_normals_texcoords[] = {
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		 
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	};
	GLuint temp_indices[] = {
		0, 1, 2,
		2, 3, 0,

		8, 9, 10,
		10, 11, 8,

		7, 6, 5,
		5, 4, 7,

		12, 13, 14,
		14, 15, 12,

		20, 21, 22,
		22, 23, 20,

		16, 17, 18,
		18, 19, 16,
	};

	// The above arrays are stored on the stack, so we shouldn't access them after the function call.
	// Instead, we'll need to malloc new arrays and copy the above to these new arrays
	int i;
	float* vertices_normals_texcoords = new float[sizeof(temp_vertices_normals_texcoords)];
	for ( i = 0; i < sizeof(temp_vertices_normals_texcoords) / sizeof(float); i++ )
		vertices_normals_texcoords[i] = temp_vertices_normals_texcoords[i];
	GLuint* indices = new GLuint[sizeof(temp_indices)];
	for ( i = 0; i < sizeof(temp_indices) / sizeof(int); i++ )
		indices[i] = temp_indices[i];

	PolyObject* obj = (PolyObject*)malloc( sizeof(PolyObject) );

	obj->textureID = LoadTexture(textureName);
	obj->vertices_normals_texcoords_size = sizeof(temp_vertices_normals_texcoords);
	obj->indices_size = sizeof(temp_indices);
	obj->vertices_normals_texcoords = vertices_normals_texcoords;
	obj->indices = indices;
	obj->translation = translation;
	obj->rotation = rotation;
	obj->scale = scale;
	SetParent(obj, parent);

	return obj;
}

// You guessed it! Creates a cylinder.
// Taken from Tutorial 7.
// NOTE: For cylinder textures, the top matches the bottom. For the side, the X texcoord is [0.0f, 1.0f], and the Y texcoord is
//	 [0.0f, 0.75f]. Top/bottom texcoords' ranges are X = [0.0f, 0.25f] and Y = [0.75f, 1.0f].
//	 The texturing won't be very accurate, I'll admit. But at least there's something.
// FIXME: There seem to be some memory issues, but unless a TA complains, I'll keep fixing them on a low priority for now. We're not using cylinders, anyway.
PolyObject* CreateCylinder(std::string textureName, PolyObject* parent, int num_height_samples, int num_circle_samples,
			glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	// If I have a problem with these parameters, I'll just scale the object.
	const float circle_radius = 0.5f;
	const float height = 3;

	const int vertex_num = (num_height_samples + 2) * num_circle_samples + 2;
	const int face_num = num_height_samples * (num_circle_samples - 1) * 2 + 2 * num_circle_samples;

	// 8 attributes for each vertex entry: position, normal, texcoord.
	float* vertices_normals_texcoords = new float[vertex_num * 8];
	GLuint* indices = new GLuint[face_num * 3];
	
	float theta;
	float h;
	float s, t;

	glm::vec3 vertex_position;
	glm::vec3 vertex_normal;
	glm::vec2 vertex_texcoord;

	for ( int j = 0; j < num_height_samples; j++ )
	{
		s = j / (float)num_height_samples;
		h = (s - 0.5f) * height;
		for ( int i = 0; i < num_circle_samples; i++ )
		{
			t = i / (float)num_circle_samples;
			theta = 2.0f * glm::pi<GLfloat>() * t;
			
			vertex_position = glm::vec3(std::cos(theta) * circle_radius, h, std::sin(theta) * circle_radius);
			vertex_normal = glm::vec3(std::cos(theta), 0.0f, std::sin(theta));
			vertex_texcoord = glm::vec2(t, s);

			// Position
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 0] = vertex_position.x;
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 1] = vertex_position.y;
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 2] = vertex_position.z;
			// Normal
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 3] = vertex_normal.x;
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 4] = vertex_normal.y;
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 5] = vertex_normal.z;
			// Texcoord
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 6] = vertex_texcoord.x;
			vertices_normals_texcoords[(j*num_circle_samples + i)*8 + 7] = vertex_texcoord.y;
			if ( j == 0 )	// Bottom face
			{
				vertex_normal = glm::vec3(0.0f, -1.0f, 0.0f);
				vertex_texcoord = glm::vec2(std::cos(theta) * 0.125f + 0.125f, std::sin(theta) * 0.125f + 0.875f);
				// Position
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 0] = vertex_position.x;
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 1] = vertex_position.y;
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 2] = vertex_position.z;
				// Normal
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 3] = vertex_normal.x;
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 4] = vertex_normal.y;
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 5] = vertex_normal.z;
				// Texcoord
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 6] = vertex_texcoord.x;
				vertices_normals_texcoords[(num_height_samples*num_circle_samples + i)*8 + 7] = vertex_texcoord.y;

			}
			if ( j == num_height_samples - 1 )	// Top face
			{
				vertex_normal = glm::vec3(0.0f, 1.0f, 0.0f);
				vertex_texcoord = glm::vec2(std::cos(theta) * 0.125f + 0.125f, std::sin(theta) * 0.125f + 0.875f);
				// Position
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 0] = vertex_position.x;
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 1] = vertex_position.y;
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 2] = vertex_position.z;
				// Normal
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 3] = vertex_normal.x;
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 4] = vertex_normal.y;
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 5] = vertex_normal.z;
				// Texcoord
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 6] = vertex_texcoord.x;
				vertices_normals_texcoords[((num_height_samples+1)*num_circle_samples + i)*8 + 7] = vertex_texcoord.y;
			}
		}
	}
	const int topvertex = num_circle_samples * (num_height_samples + 2) * 8;
	const int bottomvertex = num_circle_samples * (num_height_samples + 2) * 8 + 8;
	// Top epicenter
	vertex_position = glm::vec3(0.0f, (-0.5f + ((float)num_height_samples - 1.0f) / (float)num_height_samples)*height, 0.0f);
	vertex_normal = glm::vec3(0.0f, 1.0f, 0.0f);
	vertex_texcoord = glm::vec2(0.125f, 0.875f);
	vertices_normals_texcoords[topvertex + 0] = vertex_position.x;
	vertices_normals_texcoords[topvertex + 1] = vertex_position.y;
	vertices_normals_texcoords[topvertex + 2] = vertex_position.z;
	vertices_normals_texcoords[topvertex + 3] = vertex_normal.x;
	vertices_normals_texcoords[topvertex + 4] = vertex_normal.y;
	vertices_normals_texcoords[topvertex + 5] = vertex_normal.z;
	vertices_normals_texcoords[topvertex + 6] = vertex_texcoord.x;
	vertices_normals_texcoords[topvertex + 7] = vertex_texcoord.y;
	// Bottom epicenter
	vertex_position = glm::vec3(0.0f, (-0.5f)*height, 0.0f);
	vertex_normal = glm::vec3(0.0f, -1.0f, 0.0f);
	vertex_texcoord = glm::vec2(0.125f, 0.875f);
	vertices_normals_texcoords[bottomvertex + 0] = vertex_position.x;
	vertices_normals_texcoords[bottomvertex + 1] = vertex_position.y;
	vertices_normals_texcoords[bottomvertex + 2] = vertex_position.z;
	vertices_normals_texcoords[bottomvertex + 3] = vertex_normal.x;
	vertices_normals_texcoords[bottomvertex + 4] = vertex_normal.y;
	vertices_normals_texcoords[bottomvertex + 5] = vertex_normal.z;
	vertices_normals_texcoords[bottomvertex + 6] = vertex_texcoord.x;
	vertices_normals_texcoords[bottomvertex + 7] = vertex_texcoord.y;

	for ( int j = 0; j < num_height_samples - 1; j++ )
	{
		for ( int i = 0; i < num_circle_samples; i++ )
		{
			glm::vec3 t1(((j + 1) % num_height_samples) * num_circle_samples + i,
				j * num_circle_samples + ((i + 1) % num_circle_samples),
				j * num_circle_samples + i);
			glm::vec3 t2(((j + 1) % num_height_samples) * num_circle_samples + i,
				((j + 1) % num_height_samples) * num_circle_samples + ((i + 1) % num_circle_samples),
				j * num_circle_samples + ((i + 1) % num_circle_samples));
			for ( int k = 0; k < 3; k++ )
			{
				indices[(j*num_circle_samples + i) * 3 * 2 + k] = (GLuint)t1[k];
				indices[(j*num_circle_samples + i) * 3 * 2 + k + 3] = (GLuint)t2[k];
			}
		}
	}

	const int cylbodysize = num_circle_samples * (num_height_samples - 1) * 2;

	int j = num_height_samples;
	for ( int i = 0; i < num_circle_samples; i++ )
	{
		glm::vec3 botwedge(j*num_circle_samples + i, bottomvertex / 8, j * num_circle_samples + (i+1) % num_circle_samples);
		glm::vec3 topwedge((j + 1)*num_circle_samples + (i + 1) % num_circle_samples, topvertex / 8, (j+1)*num_circle_samples + i);
		for ( int k = 0; k < 3; k++ )
		{
			indices[(cylbodysize + i) * 3 + k] = (GLuint)topwedge[k];
			indices[(cylbodysize + i + num_circle_samples) * 3 + k] = (GLuint)botwedge[k];
		}
	}

	PolyObject* obj = (PolyObject*)malloc( sizeof(PolyObject) );

	obj->textureID = LoadTexture(textureName);
	obj->vertices_normals_texcoords_size = vertex_num * 8 * sizeof(float);
	obj->indices_size = face_num * 3 * sizeof(GLuint);
	obj->vertices_normals_texcoords = vertices_normals_texcoords;
	obj->indices = indices;
	obj->translation = translation;
	obj->rotation = rotation;
	obj->scale = scale;
	SetParent(obj, parent);

	return obj;
}

// Creates a particle system. Taken from COMP3009A tutorial 10 almost verbatim, so I won't add in the comments.
PolyObject* CreateParticleSystem(int numParticles, PolyObject* parent, glm::vec3 translation, glm::quat orientation, glm::vec3 scale)
{
	// Since this object is a particle system, there are no need for texture coordinates, but normals may still be needed.
	int attrs = 9;
	float* vertices_normals_colors = new float[numParticles * attrs];

	float trad = 0.2f;
	float maxspray = 0.5f;
	float u, v, w, theta, phi, spray;

	for (int i = 0; i < numParticles; i++)
	{
		u = ((float)rand() / RAND_MAX);
		v = ((float)rand() / RAND_MAX);
		w = ((float)rand() / RAND_MAX);
		theta = u * 2.0 * glm::pi<float>();
		phi = std::acos(2.0 * v - 1.0);
		spray = maxspray * pow((float)w, (float)(1.0 / 3.0));

		glm::vec3 normal(spray * cos(theta) * sin(phi), spray * sin(theta) * sin(phi), spray * cos(phi));
		glm::vec3 position(normal.x * trad, normal.y * trad, normal.z * trad);

		glm::vec3 color(i / (float)numParticles, 0.0, 1.0 - (i / (float)numParticles));

		vertices_normals_colors[attrs * i + 0] = position.x;
		vertices_normals_colors[attrs * i + 1] = position.y;
		vertices_normals_colors[attrs * i + 2] = position.z;
		vertices_normals_colors[attrs * i + 3] = normal.x;
		vertices_normals_colors[attrs * i + 4] = normal.y;
		vertices_normals_colors[attrs * i + 5] = normal.z;
		vertices_normals_colors[attrs * i + 6] = color.r;
		vertices_normals_colors[attrs * i + 7] = color.g;
		vertices_normals_colors[attrs * i + 8] = color.b;
	}

	PolyObject* particleSystem = (PolyObject*)malloc( sizeof(PolyObject) );

	particleSystem->vertices_normals_texcoords = vertices_normals_colors;
	particleSystem->vertices_normals_texcoords_size = numParticles * attrs;
	particleSystem->translation = translation;
	particleSystem->rotation = orientation;
	particleSystem->scale = scale;
	SetParent(particleSystem, parent);

	return particleSystem;
}

// Gets this object's transformation matrix.
glm::mat4 GetTransformationMatrix(PolyObject* obj)
{
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), obj->translation);
	glm::mat4 rotation = glm::mat4_cast(obj->rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), obj->scale);

	glm::mat4 transformation;
	if ( obj->parentObj != nullptr )
		transformation = GetTransformationMatrix(obj->parentObj) * obj->parentOrigMatrix * translation * rotation * scale;
	else
		transformation = translation * rotation * scale;
	return transformation;
}

// In addition to setting the parent object, this function also sets the inverse of the parent's transformation matrix.
// This is so that the child can be transformed before being parented, and preserve those transformations without the parent's
// affecting it..
void SetParent(PolyObject* obj, PolyObject* parent)
{
	obj->parentObj = parent;
	if ( parent != nullptr )
		obj->parentOrigMatrix = glm::inverse(GetTransformationMatrix(parent));
}

void DestroyPolyObject(PolyObject* objPtr)
{
	free(objPtr->vertices_normals_texcoords);
	free(objPtr->indices);
	free(objPtr);
}
