// Taken from mine and [REDACTED]'s base COMP3009A project code.
// It should work on Linux.

// This file is based on example 7.4 ("Camera Class") from the LearnOpenGL code repository, made by Joey de Vries.
// You can find that repository at:
//	https://github.com/JoeyDeVries/LearnOpenGL
// Some code from the COMP3009A tutorials may also be used.

// Include this file BEFORE ANYTHING ELSE!
#include <constants.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.h>
#include <utils.h>
#include <polyobject.h>
#include <baseobject.h>

#include <iostream>
#include <thread>


// ==== GLOBAL DECLARATIONS ====

GLFWwindow* gWindow;
unsigned int VAO, VBO, IBO;		// Unfortunately, for Linux compatibility, there can only be one VAO, VBO, and IBO.
unsigned int depthFBO, depthMap;	// Framebuffer object and depth map.

bool gGameIsRunning = true;		// Set to false to stop ticking, and to true to start ticking.
float current_time = 0.0f;		// Number of seconds since glfwInit().

// Camera
Camera* gCamera;
glm::vec3 cameraStartPos = glm::vec3(0.0f, -5.0f, 0.1f);
glm::vec3 cameraPos = glm::vec3(0.0f, -5.0f, 10.0f);

// Lights
glm::vec3 overheadLightPos = glm::vec3(-5.0f, -20.0f, 10.0f);
bool scoreboardLight1Active = true;
bool scoreboardLight2Active = true;

//game objects
BaseObject* paddleOne;
BaseObject* paddleTwo;
BaseObject* ball;
int score1 = 0;
int score2 = 0;

//shape name
std::string textureName1 = "container.tga";
std::string textureName2 = "container.tga";
std::string textureName3 = "balldimpled.png";

// ==== RENDERING CODE ====

// Renders a GENERIC PolyObject.
// NOTE: For some reason, generating VBOs/IBOs for each geometry object, adding the relevant data to each buffer, and using them in
//	 the render function doesn't let the object get rendered on Linux. Probably has to do with Mesa drivers. This function works
//	 around it by having only one VBO and only one IBO. It's probably less efficient, but we need the Linux compatibility, since
//	 one of us only uses Linux.
// NOTE: translation, rotation, and scale are applied AFTER the PolyObject's translation, rotation, and scale are applied.
//		 Now that I think of it, though, do we really need those inputs? They've never been used so far.
void RenderPolyObject(GLuint shader, PolyObject* obj, glm::mat4 projection, glm::mat4 view, glm::vec3 translation, glm::quat rotation,
	glm::vec3 scale, bool doShadows = false, glm::vec3 lightPos = glm::vec3(-5.0f, -20.0f, 10.0f), glm::mat4 lightProjection = glm::mat4(1.0f), glm::mat4 lightView = glm::mat4(1.0f))
{
	// First, calculate the matrices.

	glm::mat4 world;
	glm::mat4 translationMat, rotationMat, scaleMat;

	// Let's first apply the per-object translation, rotation, and scale.
	// After the per-object transformation, apply the additional transformations.
	translationMat = glm::translate(glm::mat4(1.0f), translation);
	rotationMat = glm::mat4_cast(rotation);
	scaleMat = glm::scale(glm::mat4(1.0f), scale);
	world = translationMat * rotationMat * scaleMat * GetTransformationMatrix(obj);

	// Then, deal with the rendering.

	GLint vertex_att, vsTexCoord_att, normal_att;
	GLint lightPos_uni, texture_uni, depthMap_uni;

	// Set up the shader, VAO, VBO, and IBO.
	glUseProgram(shader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, obj->vertices_normals_texcoords_size, obj->vertices_normals_texcoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indices_size, obj->indices, GL_STATIC_DRAW);

	// Shader matrices
	LoadShaderMatrix(shader, world, "world");
	LoadShaderMatrix(shader, view, "view");
	LoadShaderMatrix(shader, projection, "projection");

	// Vertex attributes
	vertex_att = glGetAttribLocation(shader, "vertex");		// Vertex position relative to the object
	glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(vertex_att);
	normal_att = glGetAttribLocation(shader, "normal");
	glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(normal_att);
	vsTexCoord_att = glGetAttribLocation(shader, "vsTexCoord");	// Texture coordinate
	glVertexAttribPointer(vsTexCoord_att, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(vsTexCoord_att);

	// Light position (only applicable for Phong and Gouraud shaders)
	lightPos_uni = glGetUniformLocation(shader, "lightPos");
	glUniform3f(lightPos_uni, lightPos.x, lightPos.y, lightPos.z);

	// Texture
	texture_uni = glGetUniformLocation(shader, "tex1");
	glUniform1i(texture_uni, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj->textureID);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Shadowing-specific code
	if (doShadows)
	{
		depthMap_uni = glGetUniformLocation(shader, "depthMap");
		glUniform1i(depthMap_uni, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		LoadShaderMatrix(shader, lightProjection, "lightProjection");
		LoadShaderMatrix(shader, lightView, "lightView");
	}

	glDrawElements(GL_TRIANGLES, obj->indices_size, GL_UNSIGNED_INT, 0);
}

// Renders an instance of BaseObject. Wrapper for RenderPolyObject.
void RenderObject(GLuint shader, BaseObject* obj, Camera* camera, bool doShadows = false, glm::vec3 lightPos = glm::vec3(-5.0f, -20.0f, 10.0f),
	glm::mat4 lightProjection = glm::mat4(1.0f), glm::mat4 lightView = glm::mat4(1.0f))
{
	PolyObject** geometry = obj->GetGeometry();

	for (int i = 0; i < obj->GetGeometryCount(); i++)
		RenderPolyObject(shader, geometry[i], camera->GetProjectionMatrix(), camera->GetViewMatrix(), obj->GetPosition(),
			obj->GetOrientation(), obj->GetScale(), doShadows, lightPos, lightProjection, lightView);
}

// Renders an instance of BaseObject. Wrapper for RenderPolyObject.
// Overloaded so that the programmer can provide the projection and view matrices.
void RenderObject(GLuint shader, BaseObject* obj, glm::mat4 projection, glm::mat4 view, bool doShadows = false, glm::vec3 lightPos = glm::vec3(-5.0f, -20.0f, 10.0f),
	glm::mat4 lightProjection = glm::mat4(1.0f), glm::mat4 lightView = glm::mat4(1.0f))
{
	PolyObject** geometry = obj->GetGeometry();

	for (int i = 0; i < obj->GetGeometryCount(); i++)
		RenderPolyObject(shader, geometry[i], projection, view, obj->GetPosition(), obj->GetOrientation(), obj->GetScale(),
			doShadows, lightPos, lightProjection, lightView);
}

#if 0	// This doesn't work, and we couldn't figure out why in time for the project submission.
// Renders a particle system. Separate from RenderPolyObject, though obviously there will be similarities.
// Taken from COMP3009A tutorial 10.
void RenderParticleSystem(GLuint shader, PolyObject* particleSystem, Camera* camera)
{
	glDisable(GL_DEPTH_TEST);

	// Enable blending, and set blending parameters.
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader);

	// First, calculate the matrices.
	glm::mat4 world = GetTransformationMatrix(particleSystem);
	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 normal = glm::transpose(glm::inverse(world));

	// Then, deal with the rendering.
	GLint vertex_att, normal_att, color_att;
	GLint time_uni;

	// Set up the shader, VAO, and VBO. IBO is not used here.
	glUseProgram(shader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, particleSystem->vertices_normals_texcoords_size, particleSystem->vertices_normals_texcoords, GL_STATIC_DRAW);

	// Set the pointers to the vertex attributes.
	// Vertex position
	vertex_att = glGetAttribLocation(shader, "vertex");
	glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(vertex_att);
	// Vertex normal
	normal_att = glGetAttribLocation(shader, "normal");
	glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(normal_att);
	// Color
	color_att = glGetAttribLocation(shader, "color");
	glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(color_att);

	// Load in the matrices to the vertex shader.
	LoadShaderMatrix(shader, world, "world");
	LoadShaderMatrix(shader, projection, "projection");
	LoadShaderMatrix(shader, view, "view");
	LoadShaderMatrix(shader, normal, "normalMat");

	// Set the uniforms for this object.
	// Current time
	time_uni = glGetUniformLocation(shader, "time");
	glUniform1f(time_uni, glfwGetTime());

	// Setup done. Draw the particles.
	glDrawArrays(GL_POINTS, 0, particleSystem->vertices_normals_texcoords_size / 9);

	// Re-enable depth testing.
	glEnable(GL_DEPTH_TEST);
}
#endif


// ==== GAME CODE ====

// This function is called for the game logic thread.
void GameThread(void)
{
	float current_time, delta;
	float last_time = glfwGetTime();
	float next_tick_time = 0.0;

	bool initVelocity = true;

	float ballDirx = 0.0025f;
	float ballDirz = 0.0025f;

	score1 = 0;
	score2 = 0;

	float moveTime = 3.0f;
	glm::vec3 cameraMoveRate = (cameraPos - cameraStartPos);
	cameraMoveRate.x *= TICKRATE / moveTime;
	cameraMoveRate.y *= TICKRATE / moveTime;
	cameraMoveRate.z *= TICKRATE / moveTime;
	glm::vec3 curCameraPos = cameraStartPos;

	int phase = 0;	// Stage of the game.
	
	// False = player 1, true = player 2.
	bool winner = false;
	int flashCount = 16;
	
	while (!glfwWindowShouldClose(gWindow))
	{
		current_time = glfwGetTime();
		delta = current_time - last_time;
		last_time = current_time;

		if (current_time < next_tick_time)
			continue;

		next_tick_time = current_time + TICKRATE;

		if (!gGameIsRunning)
			continue;

		glm::vec3 paddleOnePos = paddleOne->GetPosition();
		glm::vec3 paddleTwoPos = paddleTwo->GetPosition();
		paddleOne->SetPosition(paddleOnePos);
		paddleTwo->SetPosition(paddleTwoPos);

		glm::vec3 ballPos = ball->GetPosition();

		if (phase == 0)	// Start of the game.
		{
			// Have the camera glide from its starting position to an ideal viewing position.
			gCamera->SetCamera(curCameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			curCameraPos = curCameraPos + cameraMoveRate;
			if (std::abs(cameraPos.x - curCameraPos.x) <= 0.1f)
			{
				curCameraPos.x = cameraPos.x;
				cameraMoveRate.x = 0;
			}
			if (std::abs(cameraPos.y - curCameraPos.y) <= 0.1f)
			{
				curCameraPos.y = cameraPos.y;
				cameraMoveRate.y = 0;
			}
			if (std::abs(cameraPos.z - curCameraPos.z) <= 0.1f)
			{
				curCameraPos.z = cameraPos.z;
				cameraMoveRate.z = 0;
			}

			if (curCameraPos == cameraPos)
			{
				phase = 1;
				next_tick_time = current_time + 3.0f;	// Wait 3 seconds to start the game.
			}
		}
		else if (phase == 1)	// Game loop.
		{
			ballPos.x += ballDirx;
			ballPos.z += ballDirz;
			ball->SetPosition(ballPos);

			ball->Collision(paddleOnePos, paddleTwoPos, ballDirx, ballDirz, score1, score2);

			if (score1 == 11 || score2 == 11) {
				if (score1 > score2) {
					std::cout << "Player 1 has WON!!" << std::endl;
					winner = false;
				}
				else {
					std::cout << "Player 2 has WON!!" << std::endl;
					winner = true;
				}
				//glfwSetWindowShouldClose(gWindow, true);
				phase = 2;
			}
		}
		else if (phase == 2)	// End of the game.
		{
			// Flash the scoreboard corresponding to the winner 8 times.
			if (!winner)
				scoreboardLight1Active = !scoreboardLight1Active;
			else
				scoreboardLight2Active = !scoreboardLight2Active;

			if (flashCount == 0)
				glfwSetWindowShouldClose(gWindow, true);

			flashCount--;
			next_tick_time = current_time + 0.25f;	// Each flash is half of a second.
		}
	}
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Quit the program when pressing 'q'
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	bool check = true;

	if (glfwGetKey(window, GLFW_KEY_A) && glfwGetKey(window, GLFW_KEY_LEFT)) {
		paddleOne->SetPosition(glm::vec3((float)paddleOne->GetPosition().x + 0.1, (float)paddleOne->GetPosition().y, (float)paddleOne->GetPosition().z));
		paddleTwo->SetPosition(glm::vec3((float)paddleTwo->GetPosition().x + 0.1, (float)paddleTwo->GetPosition().y, (float)paddleTwo->GetPosition().z));
		check = false;
	}
	if (glfwGetKey(window, GLFW_KEY_A) && glfwGetKey(window, GLFW_KEY_RIGHT)) {
		paddleOne->SetPosition(glm::vec3((float)paddleOne->GetPosition().x + 0.1, (float)paddleOne->GetPosition().y, (float)paddleOne->GetPosition().z));
		paddleTwo->SetPosition(glm::vec3((float)paddleTwo->GetPosition().x - 0.1, (float)paddleTwo->GetPosition().y, (float)paddleTwo->GetPosition().z));
		check = false;
	}

	if (glfwGetKey(window, GLFW_KEY_D) && glfwGetKey(window, GLFW_KEY_LEFT)) {
		paddleOne->SetPosition(glm::vec3((float)paddleOne->GetPosition().x - 0.1, (float)paddleOne->GetPosition().y, (float)paddleOne->GetPosition().z));
		paddleTwo->SetPosition(glm::vec3((float)paddleTwo->GetPosition().x + 0.1, (float)paddleTwo->GetPosition().y, (float)paddleTwo->GetPosition().z));
		check = false;
	}
	if (glfwGetKey(window, GLFW_KEY_D) && glfwGetKey(window, GLFW_KEY_RIGHT)) {
		paddleOne->SetPosition(glm::vec3((float)paddleOne->GetPosition().x - 0.1, (float)paddleOne->GetPosition().y, (float)paddleOne->GetPosition().z));
		paddleTwo->SetPosition(glm::vec3((float)paddleTwo->GetPosition().x - 0.1, (float)paddleTwo->GetPosition().y, (float)paddleTwo->GetPosition().z));
		check = false;
	}

	//paddleOne
	if (check) {
		if (glfwGetKey(window, GLFW_KEY_A)) {
			paddleOne->SetPosition(glm::vec3((float)paddleOne->GetPosition().x + 0.1, (float)paddleOne->GetPosition().y, (float)paddleOne->GetPosition().z));
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			paddleOne->SetPosition(glm::vec3((float)paddleOne->GetPosition().x - 0.1, (float)paddleOne->GetPosition().y, (float)paddleOne->GetPosition().z));
		}
	}


	//paddleTwo controls
	if (check) {
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			paddleTwo->SetPosition(glm::vec3((float)paddleTwo->GetPosition().x + 0.1, (float)paddleTwo->GetPosition().y, (float)paddleTwo->GetPosition().z));
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			paddleTwo->SetPosition(glm::vec3((float)paddleTwo->GetPosition().x - 0.1, (float)paddleTwo->GetPosition().y, (float)paddleTwo->GetPosition().z));
		}
	}
}
// Callback for when the window is resized
void ResizeCallback(GLFWwindow* window, int width, int height) {

	// Set OpenGL viewport based on framebuffer width and height
	glViewport(0, 0, width, height);

	// Update projection matrix
	void* ptr = glfwGetWindowUserPointer(window);
	glm::mat4* projection_matrix = (glm::mat4*)ptr;
	float top = tan((gCamera->getFieldOfView() / 2.0) * (glm::pi<float>() / 180.0)) * (gCamera->getNearPlane());
	float right = top * width / height;
	(*projection_matrix) = glm::frustum(-right, right, -top, top, gCamera->getNearPlane(), gCamera->getFarPlane());
}


// ==== MAIN FUNCTION ====

int main(int argc, char** argv)
{
	// Init GLFW.
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!\n";
		return -1;
	}

	// Try to use OpenGL 3.3, in case future versions break something, and use the core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gWindow = glfwCreateWindow(WINWIDTH, WINHEIGHT, WINNAME, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cout << "Failed to create GLFW Window!\n";
		return -2;
	}

	// Set the window context, and set up event hooks.
	glfwMakeContextCurrent(gWindow);
	// TODO: Hook to event handlers here.

	// Init GLEW.
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW!\n";
		return -3;
	}

	// Print the version info.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);

	// Cubes' faces seem to be A-OK now, so face culling has been enabled.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//positions for each paddle & ball
	glm::vec3 posiOne = { 0.0f ,-0.5f ,3.35f };
	glm::vec3 posiTwo = { 0.0f, -0.5f, -3.35f };
	glm::vec3 posiBall = { 0.0f, -0.5f, 0.0f };

	//orientation of each paddle & ball
	glm::quat orientation1 = { 0,0,0,1 };
	glm::quat orientation2 = { 0,0,0,1 };
	glm::quat orientationBall = { 0,0,0,1 };


	//scale of each paddle & ball
	glm::vec3 scale1 = { 1.0f,0.5f,0.5f };
	glm::vec3 scale2 = { 1.0f,0.5f,0.5f };
	glm::vec3 scaleBall = { 0.5f,0.5f,0.5f };

	// Callbacks
	glfwSetKeyCallback(gWindow, KeyCallback);
	glfwSetFramebufferSizeCallback(gWindow, ResizeCallback);

	// Game camera. Starts at one position and glides to an ideal viewing position.
	gCamera = new Camera();
	gCamera->SetCamera(cameraStartPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	// Generate the depth framebuffer, initialize the depth map, then bind the depth map to the depth framebuffer.
	glGenFramebuffers(1, &depthFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//GLuint unlitTexShader = LoadShaders("unlitTex");
	//GLuint phongTexShader = LoadShaders("phongTex");
	GLuint depthShader = LoadShaders("depth");
	GLuint phongTexShadowShader = LoadShaders("phongTexShadow");
#if 0	// Particles aren't working. Believe us, we tried.
	GLuint particle = LoadShaders("particle");
#endif

	// Game objects
	PolyObject* platform = CreateCube("court.tga", nullptr, glm::vec3(0.0f, 0.5f, 0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(10.0f, 0.1f, 10.0f));
	paddleOne = new BaseObject(posiOne,orientation1, scale1,&textureName1, true);
	paddleTwo = new BaseObject(posiTwo,orientation2,scale2, &textureName1, true);
	ball = new BaseObject(posiBall, orientationBall, scaleBall, &textureName3, false);

	// Scoreboards
	PolyObject* scoreboard1 = CreateCube("checkerboard_red.tga", nullptr, glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(3.5f, 3.0f, 0.1f));
	PolyObject** points1 = (PolyObject**)malloc( sizeof(PolyObject*) * 11 );
	points1[0] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(1.125f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[1] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(0.375f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[2] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(-0.375f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[3] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(-1.125f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[4] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(1.125f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[5] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(0.375f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[6] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(-0.375f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[7] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(-1.125f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[8] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(1.125f, -4.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[9] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(0.375f, -4.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points1[10] = CreateCube("checkerboard_green.tga", scoreboard1, glm::vec3(-0.375f, -4.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	scoreboard1->translation.x = 3.0f;

	PolyObject* scoreboard2 = CreateCube("checkerboard_blue.tga", nullptr, glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(3.5f, 3.0f, 0.1f));
	PolyObject** points2 = (PolyObject**)malloc(sizeof(PolyObject*) * 11);
	points2[0] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(1.125f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[1] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(0.375f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[2] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(-0.375f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[3] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(-1.125f, -6.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[4] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(1.125f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[5] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(0.375f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[6] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(-0.375f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[7] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(-1.125f, -5.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[8] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(1.125f, -4.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[9] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(0.375f, -4.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	points2[10] = CreateCube("checkerboard_green.tga", scoreboard2, glm::vec3(-0.375f, -4.0f, 1.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	scoreboard2->translation.x = -3.0f;
	
	// Start the game thread.
	std::thread theGameThread(GameThread);

	glm::mat4 lightProjection1, lightView1;
	glm::mat4 lightProjection2, lightView2;
	glm::mat4 lightProjection3, lightView3;
	float near = 1.0f, far = 50.0f;
	glm::vec3 position;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(gWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// There are three orthogonal lights, which means three depth passes and three rendering passes.

		// == DEPTH PASS 1 ==
		// Gets the depth map for the table, paddles, and ball.
		// We include the scoreboards in the depth map, since there are objects upon which shadows will be cast.

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Light from above, slightly sideways.
		lightProjection1 = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
		lightView1 = glm::lookAt(overheadLightPos, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glCullFace(GL_FRONT);	// Frontface culling while generating the depth map mitigates Peter Panning (detachment) of the shadows.
		RenderObject(depthShader, paddleOne, lightProjection1, lightView1, false);
		RenderObject(depthShader, paddleTwo, lightProjection1, lightView1, false);
		RenderObject(depthShader, ball, lightProjection1, lightView1, false);
		RenderPolyObject(depthShader, platform, lightProjection1, lightView1, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		RenderPolyObject(depthShader, scoreboard1, lightProjection1, lightView1, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		for (int i = 0; i < score1; i++)
			RenderPolyObject(depthShader, points1[i], lightProjection1, lightView1, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		RenderPolyObject(depthShader, scoreboard2, lightProjection1, lightView1, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		for (int i = 0; i < score2; i++)
			RenderPolyObject(depthShader, points2[i], lightProjection1, lightView1, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// == RENDER PASS 1 ==
		// Renders the table, paddles, and ball, with shadows.

		glViewport(0, 0, WINWIDTH, WINHEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderObject(phongTexShadowShader, paddleOne, gCamera, true, overheadLightPos, lightProjection1, lightView1);
		RenderObject(phongTexShadowShader, paddleTwo, gCamera, true, overheadLightPos, lightProjection1, lightView1);
		RenderObject(phongTexShadowShader, ball, gCamera, true, overheadLightPos, lightProjection1, lightView1);
		RenderPolyObject(phongTexShadowShader, platform, gCamera->GetProjectionMatrix(), gCamera->GetViewMatrix(), glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f),
			true, overheadLightPos, lightProjection1, lightView1);

		// == DEPTH PASS 2 ==
		// Gets the depth map for the first scoreboard.
		// The score pips are also depth-tested and cast shadows on the scoreboards even if they're not being rendered.
		// This is to give the players an idea of the total score ahead of time.

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Scoreboard 1 light
		lightProjection2 = glm::ortho(-3.5f, 3.5f, -3.5f, 3.5f, near, far);
		// Take away the light if the scoreboard light's not active.
		position = scoreboardLight1Active ? glm::vec3(3.0f, -5.0f, 5.0f) : glm::vec3(9999);
		lightView2 = glm::lookAt(position, glm::vec3(3.0f, -5.0f, 0.0f), glm::vec3(0, 1, 0));
		glCullFace(GL_FRONT);
		RenderPolyObject(depthShader, scoreboard1, lightProjection2, lightView2, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		for (int i = 0; i < 11; i++)
			RenderPolyObject(depthShader, points1[i], lightProjection2, lightView2, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// == RENDER PASS 2 ==
		// Renders the first scoreboard, with shadows.

		glViewport(0, 0, WINWIDTH, WINHEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);	// Don't clear the color buffer; we're not yet done drawing..

		RenderPolyObject(phongTexShadowShader, scoreboard1, gCamera->GetProjectionMatrix(), gCamera->GetViewMatrix(), glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f),
			true, glm::vec3(3.0f, -5.0f, 5.0f), lightProjection2, lightView2);
		for (int i = 0; i < score1; i++)
			RenderPolyObject(phongTexShadowShader, points1[i], gCamera->GetProjectionMatrix(), gCamera->GetViewMatrix(), glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f),
				true, glm::vec3(3.0f, -5.0f, 5.0f), lightProjection2, lightView2);

		// == DEPTH PASS 3 ==
		// Gets the depth map for the second scoreboard.
		// The score pips are also depth-tested and cast shadows on the scoreboards even if they're not being rendered.
		// This is to give the players an idea of the total score ahead of time.

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Scoreboard 2 light
		lightProjection3 = glm::ortho(-3.5f, 3.5f, -3.5f, 3.5f, near, far);
		// Take away the light if the scoreboard light's not active.
		position = scoreboardLight2Active ? glm::vec3(-3.0f, -5.0f, 5.0f) : glm::vec3(9999);
		lightView3 = glm::lookAt(position, glm::vec3(-3.0f, -5.0f, 0.0f), glm::vec3(0, 1, 0));
		glCullFace(GL_FRONT);
		RenderPolyObject(depthShader, scoreboard2, lightProjection3, lightView3, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		for (int i = 0; i < 11; i++)
			RenderPolyObject(depthShader, points2[i], lightProjection3, lightView3, glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f), false);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// == RENDER PASS 3 ==
		// Renders the second scoreboard, with shadows.

		glViewport(0, 0, WINWIDTH, WINHEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);

		RenderPolyObject(phongTexShadowShader, scoreboard2, gCamera->GetProjectionMatrix(), gCamera->GetViewMatrix(), glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f),
			true, glm::vec3(3.0f, -5.0f, 5.0f), lightProjection3, lightView3);
		for (int i = 0; i < score2; i++)
			RenderPolyObject(phongTexShadowShader, points2[i], gCamera->GetProjectionMatrix(), gCamera->GetViewMatrix(), glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f),
				true, glm::vec3(3.0f, -5.0f, 5.0f), lightProjection3, lightView3);

		glfwSwapBuffers(gWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	// FIXME: Abort signal gets called when this function returns. Not sure why, and the program's being closed anyway, so this issue is a low priority.
	//		  But don't run this game with elevated privileges, just in case this issue's caused due to stack smashing.
	return 0;
}
