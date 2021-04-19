#ifndef BASEOBJECT_H
#define BASEOBJECT_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <polyobject.h>
#include <string>
// Base object. All other game objects must be derived from this.
// NOTE: This is technically supposed to be an abstract class, but instantiation is allowed and accounted for.
class BaseObject
{
public:
	// NOTE: It's assumed the one creating this object will know how many objects there are.
	// Subclasses MUST NOT call these constructors. Calling the destructor is fine.
	BaseObject(glm::vec3 position, glm::quat orientation, glm::vec3 scale, std::string* textureNames, bool check);
	BaseObject();
	~BaseObject();
	glm::vec3 GetPosition(void);
	glm::quat GetOrientation(void);
	glm::vec3 GetScale(void);
	void SetPosition(glm::vec3 position);
	void SetOrientation(glm::quat orientation);
	void SetScale(glm::vec3 scale);
	int GetGeometryCount(void);




	void Collision(glm::vec3 position1, glm::vec3 position2, float&, float&, int&, int&);

	PolyObject** GetGeometry(void);

	void Tick(float time_delta);	// Called every game tick.

private:
protected:
	int m_iObjCount;
	PolyObject** m_pGeometry;
	// NOTE: The following are applied to the topmost parent. There SHOULD be at least one object.
	glm::vec3 m_vecPosition;
	glm::quat m_quatOrientation;
	glm::vec3 m_vecScale;
	bool padOrBall;
};
#endif	// BASEOBJECT_H