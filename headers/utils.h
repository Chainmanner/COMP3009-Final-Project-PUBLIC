#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

typedef struct Texture {
	int width;
	int height;
	int nrChannels;
	void* data;
} Texture;

// Loads a texture.
GLuint LoadTexture(std::string filename);

#if 0	// These functions are part of an overall hack that shouldn't be necessary on Windows.
// Creates the texture data array.
void AllocateTextureBuffer(int numTextures);

// Gets a texture's data from memory, given a texture ID.
Texture GetTextureData(unsigned int textureID);

// Deallocates all loaded textures.
void DeallocateTextureBuffer(void);
#endif

// Loads a text file into a string.
std::string LoadTextFile(std::string filename);

// Loads vertex and pixel shaders of one name.
GLuint LoadShaders(std::string shaderName);

// Loads a matrix into a shader uniform.
void LoadShaderMatrix(GLuint shader, glm::mat4 matrix, std::string name);

#endif	// UTILS_H
