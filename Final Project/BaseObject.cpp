#include "stdafx.h"

#include "BaseObject.h"



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