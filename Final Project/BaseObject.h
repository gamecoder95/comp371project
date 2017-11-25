#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "Shader.h"

struct BoundingBox
{
	float back; // -z
	float front; // +z
	float right; // +x
	float left; // -x
	float bottom; // -y
	float top; // +y
};

class BaseObject
{
protected:
	Shader* shader;
	BoundingBox collisionBox;
	glm::vec3 position;

	virtual void setCollisionBox() = 0;
	

public:
	BaseObject(Shader* shader);
	~BaseObject();

	virtual float getBack() const;
	virtual float getFront() const;
	virtual float getRight() const;
	virtual float getLeft() const;
	virtual float getTop() const;
	virtual float getBottom() const;
	virtual bool isCollision(const BaseObject& other);

};

#endif