#include <constants.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
//#include <stb_image.h>
#include <utils.h>
#include <string>
#include <fstream>
#include <cstdlib>

GLuint LoadTexture(std::string filename)
{
	std::cout << "Loading " << TEXTURES_DIR << filename << "... ";

	// Create texture string name
	std::string texture_name = std::string(TEXTURES_DIR) + filename;

	//Load the texture using the SOIL library, and create a new ID for it
	GLuint texture = SOIL_load_OGL_texture(texture_name.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	//error handling in case the texture wasn't loaded
	if (!texture) {
		std::cout << "Failed! " << SOIL_last_result() << "\n";
		throw(std::ios_base::failure(std::string("Error loading texture ") + std::string(texture_name) + std::string(": ") + std::string(SOIL_last_result())));
	}

	std::cout << "Loaded.\n";
	// Create resource
	return texture;
}

#if 0	// TODO: If the Windows approach works, remove this block.
unsigned int gCurTexID = 1;
Texture* gTextureData = nullptr;

// Creates the texture data array.
void AllocateTextureBuffer(int numTextures)
{
	gTextureData = (Texture*)malloc(sizeof(Texture) * numTextures);
}

// Loads a texture. Provided by Shar, our friendly neighborhood COMP3009A TA. A few modifications were made.
GLuint LoadTexture(std::string filename)
{
	std::string texture_name = TEXTURES_DIR + filename;
	const char* file = texture_name.c_str();
	GLuint texture = gCurTexID;

	// Texture wrapping and filtering options.
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 3);
	if (data)
	{
		/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);*/
		gTextureData[gCurTexID].width = width;
		gTextureData[gCurTexID].height = height;
		gTextureData[gCurTexID].nrChannels = nrChannels;
		gTextureData[gCurTexID].data = data;
		gCurTexID++;
	}
	else
	{
		std::cout << "Failed to load texture " << filename << "!\n";
		return GL_FALSE;
	}
	//stbi_image_free(data);

	return texture;
}

// Gets a texture's data from memory. I hate to do this - it's likely not as efficient as the standard way of loading textures - but
// only this way will multiple textures be shown on Linux. Otherwise, the last loaded texture will be shown. There may be a better way
// that'll also work on Linux, but I don't have time to find it.
Texture GetTextureData(unsigned int textureID)
{
	return gTextureData[textureID];
}

// Deallocates all loaded textures.
void DeallocateTextureBuffer(void)
{
	for ( int i = 0; i < gCurTexID; i++ )
		stbi_image_free( gTextureData[i].data );
	free(gTextureData);
}
#endif

//This function is used to read the shader programs. OpenGL does not read them as a
//specific file type, but instead as simply a text file containing c code. 
std::string LoadTextFile(std::string filename) {

	const char* char_file = filename.c_str();
	std::ifstream f;
	f.open(char_file);
	if (f.fail()) {
		throw(std::ios_base::failure(std::string("Error opening file ") + std::string(filename)));
	}

	std::string content;
	std::string line;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	f.close();

	return content;
}

//This function loads the fragment and vertex shader. 
// NOTE: Taken from COMP3009A tutorials, with minor adjustments. Recent as of tutorial 10.
GLuint LoadShaders(std::string shaderName) {

	// Create a shader from vertex program source code
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	//Read in the shaders into strings
	std::string fragment_shader = LoadTextFile(std::string(SHADERS_DIR) + shaderName + ".frag");
	std::string vertex_shader = LoadTextFile(std::string(SHADERS_DIR) + shaderName + ".vert");

	//Compile the vertex shader from the source string
	const char* const_vertex_shader = vertex_shader.c_str();
	glShaderSource(vs, 1, &const_vertex_shader, NULL);
	glCompileShader(vs);

	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
	}

	//Compile the fragment shader from the source string
	const char* const_fragment_shader = fragment_shader.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &const_fragment_shader, NULL);
	glCompileShader(fs);

	// Check if shader compiled successfully
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
	}

	// Try to also load a geometry shader
	bool geometry_program = false;

	std::string geometry_shader;
	GLuint gs;
	try {
		geometry_shader = LoadTextFile(std::string(SHADERS_DIR) + shaderName + ".geom");
		geometry_program = true;
	}
	catch (std::exception& e) {
	}

	if (geometry_program) {
		// Create a shader from the geometry program source code
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		const char* source_gp = geometry_shader.c_str();
		glShaderSource(gs, 1, &source_gp, NULL);
		glCompileShader(gs);

		// Check if shader compiled successfully
		GLint status;
		glGetShaderiv(gs, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(gs, 512, NULL, buffer);
			throw(std::ios_base::failure(std::string("Error compiling geometry shader: ") + std::string(buffer)));
		}
	}

	// Create a shader program linking both vertex and fragment shaders together
	GLuint shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	if (geometry_program) {
		glAttachShader(shader, gs);
	}
	glLinkProgram(shader);

	// Check if shaders were linked successfully
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
	}

	// Delete memory used by shaders, since they were already compiled
	// and linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	return shader;

}

//This function sends the matrix into the uniform in the shader
//uniforms are attributes that are defined in the shader but can be
//accessed outside it.
// NOTE: Taken from COMP3009A tutorials.yy
void LoadShaderMatrix(GLuint shader, glm::mat4 matrix, std::string name) {

        //First get the uniform from the shader by specifying its name
        GLint shader_mat = glGetUniformLocation(shader, name.c_str());

        //Now load the matrix using the proper uniform (matrix4fv) function.
        glUniformMatrix4fv(shader_mat, 1, GL_FALSE, glm::value_ptr(matrix));
}
