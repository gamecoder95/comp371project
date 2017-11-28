#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "stdafx.h"

#include "Shader.h"

//struct BoundingBox
//{
//	float back; // -z
//	float front; // +z
//	float right; // +x
//	float left; // -x
//	float bottom; // -y
//	float top; // +y
//
//	void scale(float scale)
//	{
//		back *= scale;
//		front *= scale;
//		left *= scale;
//		right *= scale;
//		bottom *= scale;
//		top *= scale;
//	}
//};

class BaseObject
{
protected:
	static const GLfloat SCALE;
	Shader* shader;
	//BoundingBox collisionBox;
	glm::vec3 position;
	float radius;

	// Meant to be overriden by those that need it!
	virtual void setCollisionRadius();

public:
	BaseObject(Shader* shader);
	~BaseObject();

	//virtual float getBack() const;
	//virtual float getFront() const;
	//virtual float getRight() const;
	//virtual float getLeft() const;
	//virtual float getTop() const;
	//virtual float getBottom() const;
	virtual bool isCollision(const BaseObject& other);
	virtual void update() = 0;
	virtual bool isDestroyed() = 0;
	virtual void destroy() = 0;

	// Meant to be overriden by those that need it!
	virtual void onCollision(BaseObject& other);
	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getRadiusVect(const glm::vec3& point) const;
	virtual float getRadius() const;

	static float getLengthVector(const glm::vec3& vect);
};

#endif