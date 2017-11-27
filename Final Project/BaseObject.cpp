#include "stdafx.h"

#include "BaseObject.h"

// Static constants here
const GLfloat BaseObject::SCALE = 0.1f;

BaseObject::BaseObject(Shader* shader)
{
	this->shader = shader;
}

BaseObject::~BaseObject()
{

}

float BaseObject::getBack() const
{
	return position.z + collisionBox.back;
}

float BaseObject::getFront() const
{
	return position.z + collisionBox.front;
}

float BaseObject::getRight() const
{
	return position.x + collisionBox.right;
}

float BaseObject::getLeft() const
{
	return position.x + collisionBox.left;
}

float BaseObject::getTop() const
{
	return position.y + collisionBox.top;
}

float BaseObject::getBottom() const
{
	return position.y + collisionBox.bottom;
}

// Meant to be overriden by those that need it!
void BaseObject::setCollisionBox()
{
	collisionBox.back = 0.0f;
	collisionBox.front = 0.0f;
	collisionBox.left = 0.0f;
	collisionBox.right = 0.0f;
	collisionBox.top = 0.0f;
	collisionBox.bottom = 0.0f;
}

// Meant to be overriden by those that need it!
void BaseObject::onCollision(BaseObject& other)
{
	// Does nothing here, needs definition by those that need it!
}

bool BaseObject::isCollision(const BaseObject& other)
{
	if (other.getBack() > this->getFront()
		|| other.getFront() < this->getBack()
		|| other.getRight() < this->getLeft()
		|| other.getLeft() > this->getRight()
		|| other.getTop() < this->getBottom()
		|| other.getBottom() > this->getTop())
	{
		return false;
	}
	else
	{
		return true;
	}
}

glm::vec3 BaseObject::getPosition()
{
	return position;
}