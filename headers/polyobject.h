#ifndef POLYOBJECT_H
#define POLYOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// A polygon object.
// Could have named it "Geometry", but I didn't want to confuse it for the same struct in the COMP3009A1 tutorials.
typedef struct PolyObject {
	int textureID;
	int vertices_normals_texcoords_size;
	int indices_size;
	float* vertices_normals_texcoords;
	GLuint* indices;
	PolyObject* parentObj;
	glm::mat4 parentOrigMatrix;
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
} PolyObject;

PolyObject* CreateCube(std::string textureName, PolyObject* parent = nullptr, glm::vec3 translation = glm::vec3(0.0f),
			glm::quat rotation = glm::quat(glm::vec3(0.0f)), glm::vec3 scale = glm::vec3(1.0f));

PolyObject* CreateCylinder(std::string textureName, PolyObject* parent = nullptr, int num_height_samples = 2,
			int num_circle_samples = 100, glm::vec3 translation = glm::vec3(0.0f),
			glm::quat rotation = glm::quat(glm::vec3(0.0f)), glm::vec3 scale = glm::vec3(1.0f));

PolyObject* CreateParticleSystem(int numParticles = 1000, PolyObject* parent = nullptr, glm::vec3 translation = glm::vec3(0.0f),
			glm::quat orientation = glm::quat(glm::vec3(0.0f)), glm::vec3 scale = glm::vec3(1.0f));

glm::mat4 GetTransformationMatrix(PolyObject* obj);

void SetParent(PolyObject* obj, PolyObject* parent);

void DestroyPolyObject(PolyObject* objPtr);

#endif	// POLYOBJECT_H
