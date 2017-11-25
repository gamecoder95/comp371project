#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Shader* shader, const glm::vec3& pos, const glm::vec3& dir, const Color& col)
{
	this->shader = shader;
	setColor(col);
	setPosition(pos);
	setDirection(dir);
}

void DirectionalLight::setColor(const Color& col)
{
	color = col;
	shader->setVec3("light_color.ambient", color.ambient);
	shader->setVec3("light_color.diffuse", color.diffuse);
	shader->setVec3("light_color.specular", color.specular);
}

void DirectionalLight::setPosition(const glm::vec3& pos)
{
	position = pos;
	shader->setVec3("light_position", position);
}

void DirectionalLight::setDirection(const glm::vec3& dir)
{
	direction = dir;
	shader->setVec3("light_direction", direction);
}

Color DirectionalLight::getColor() const
{
	return color;
}

glm::vec3 DirectionalLight::getPosition() const
{
	return direction;
}

glm::vec3 DirectionalLight::getDirection() const
{
	return direction;
}

DirectionalLight::~DirectionalLight()
{
}
