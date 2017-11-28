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

//float BaseObject::getBack() const
//{
//	return position.z + collisionBox.back;
//}
//
//float BaseObject::getFront() const
//{
//	return position.z + collisionBox.front;
//}
//
//float BaseObject::getRight() const
//{
//	return position.x + collisionBox.right;
//}
//
//float BaseObject::getLeft() const
//{
//	return position.x + collisionBox.left;
//}
//
//float BaseObject::getTop() const
//{
//	return position.y + collisionBox.top;
//}
//
//float BaseObject::getBottom() const
//{
//	return position.y + collisionBox.bottom;
//}

// Meant to be overriden by those that need it!
void BaseObject::setCollisionRadius()
{
	radius = 0.0f;
	//collisionBox.back = 0.0f;
	//collisionBox.front = 0.0f;
	//collisionBox.left = 0.0f;
	//collisionBox.right = 0.0f;
	//collisionBox.top = 0.0f;
	//collisionBox.bottom = 0.0f;
}

// Meant to be overriden by those that need it!
void BaseObject::onCollision(BaseObject& other)
{
	// Does nothing here, needs definition by those that need it!
}

bool BaseObject::isCollision(const BaseObject& other)
{
	// Get distance between sphere origins
	float center_dist = BaseObject::getLengthVector(other.getPosition() - getPosition());

	// Get the distance between the radii
	float dist_diff = center_dist - other.getRadius() - radius;

	// If the difference between the radii is negative, then the objects are touching or within each
	// other's collision spheres.
	return dist_diff <= 0.0f;

	//if (other.getBack() > this->getFront()
	//	|| other.getFront() < this->getBack()
	//	|| other.getRight() < this->getLeft()
	//	|| other.getLeft() > this->getRight()
	//	|| other.getTop() < this->getBottom()
	//	|| other.getBottom() > this->getTop())
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;
	//}
}

glm::vec3 BaseObject::getPosition() const
{
	return position;
}

glm::vec3 BaseObject::getRadiusVect(const glm::vec3& point) const
{
	return getPosition() + radius * glm::normalize(point - getPosition());
}

float BaseObject::getRadius() const
{
	return radius;
}

float BaseObject::getLengthVector(const glm::vec3& vect)
{
	return sqrt(glm::dot(vect, vect));
}