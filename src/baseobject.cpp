#include <constants.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <polyobject.h>
#include <baseobject.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
BaseObject::BaseObject(glm::vec3 position, glm::quat orientation, glm::vec3 scale, std::string* textureName, bool check)
{
	m_iObjCount = 1;
	m_pGeometry = new PolyObject * [m_iObjCount];
	m_pGeometry[0] = CreateCube(textureName[0], nullptr, position, orientation, scale);

	m_vecPosition = position;
	m_quatOrientation = orientation;
	m_vecScale = scale;
	this->padOrBall = check;
}

BaseObject::BaseObject()
{
	m_vecPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_quatOrientation = glm::quat(glm::vec3(0, 0, 0));
	m_vecScale = glm::vec3(1.0f, 1.0f, 1.0f);
}

BaseObject::~BaseObject()
{
	for (int i = 0; i < m_iObjCount; i++)
		DestroyPolyObject(m_pGeometry[i]);
}

glm::vec3 BaseObject::GetPosition(void)
{
	return m_vecPosition;
}

glm::quat BaseObject::GetOrientation(void)
{
	return m_quatOrientation;
}

glm::vec3 BaseObject::GetScale(void)
{
	return m_vecScale;
}

void BaseObject::SetPosition(glm::vec3 position)
{

	if (this->padOrBall) {
		//is a paddle
		if (position.x > 4.5f) {
			position.x = 4.5f;
		}
		else if (position.x < -4.5f) {
			position.x = -4.5f;
		}
	}

	m_vecPosition = position;

	/*if ( m_pGeometry[0] != NULL )
		m_pGeometry[0]->translation = position;*/
}

void BaseObject::SetOrientation(glm::quat orientation)
{
	m_quatOrientation = orientation;
	/*if ( m_pGeometry[0] != NULL )
		m_pGeometry[0]->rotation = orientation;*/
}

void BaseObject::SetScale(glm::vec3 scale)
{
	m_vecScale = scale;
	/*if ( m_pGeometry[0] != NULL )
		m_pGeometry[0]->scale = scale;*/
}

int BaseObject::GetGeometryCount(void)
{
	return m_iObjCount;
}

PolyObject** BaseObject::GetGeometry(void)
{
	return m_pGeometry;
}

void BaseObject::Collision(glm::vec3 position1, glm::vec3 position2, float& ballDirx, float& ballDirz, int& score1, int& score2) {

	glm::vec3 ballPos = this->GetPosition();

	float randomDiff = (rand() % 1000) / (float)RAND_MAX;
	//std::cout << "random: " << randomDiff << std::endl;
	/*if (abs(position1.x - ballPos.x) > 5 || abs(position2.x - ballPos.x) > 5) {
		return;
	}*/
	if (ballPos.x > 4.65f) {
		ballDirx *= -1.05;
	}
	else if (ballPos.x < -4.65f) {
		ballDirx *= -1.05;
	}


	float currTime;
	float next;
	float sum = 0.0f;

	if (ballPos.z > 4.8f && ballPos.z < 5.0f) {
		if (ballPos.x < (position1.x - 1.15f) || ballPos.x >(position1.x + 1.15f)) {
			score2++;
			currTime = glfwGetTime();

			next = currTime + 4000 * TICKRATE;
			this->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
			while (currTime < next) {
				currTime = glfwGetTime();
			}

			ballDirx = 0.0025f;
			ballDirz = 0.0025f;
			//std::cout << "Score 1: " << score1 << "Score2: " << score2 << std::endl;
			return;
		}
		else {
			//ballDirx += 2 * randomDiff - 0.01f;
			ballDirx *= -1.01;
			ballDirz *= -1.01;
			return;
		}
	}
	else if (ballPos.z <-4.8f && ballPos.z>-5.0f) {
		sum = 0.0f;
		if (ballPos.x < (position2.x - 1.15f) || ballPos.x >(position2.x + 1.15f)) {
			score1++;
			currTime = glfwGetTime();
			next = currTime + 4000 * TICKRATE;
			this->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));

			//delay time between spawns
			while (currTime < next) {
				currTime = glfwGetTime();
			}

			ballDirx = 0.0025f;
			ballDirz = 0.0025f;
			//std::cout << "Score 1: " << score1 << "Score2: " << score2 << std::endl;
			return;
		}
		else {
			//ballDirx += 2 * randomDiff - 0.01f;
			ballDirx *= -1.01;

			ballDirz *= -1.01;
			return;
		}
	}
}
void BaseObject::Tick(float time_delta)
{
	// Nothing to be done for the base object.
}
