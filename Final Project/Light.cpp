#include "Light.h"

Light::Light(Shader* shader, const glm::vec3& pos, const Color& col)
	: BaseObject(shader)
{
	setColor(col);
	setPosition(pos);
}

void Light::setColor(const Color& col)
{
	color = col;
	shader->setVec3("light_color.ambient", color.ambient);
	shader->setVec3("light_color.diffuse", color.diffuse);
	shader->setVec3("light_color.specular", color.specular);
}

void Light::setPosition(const glm::vec3& pos)
{
	position = pos;
	shader->setVec3("light_position", position);
}

Color Light::getColor() const
{
	return color;
}

glm::vec3 Light::getPosition() const
{
	return position;
}

Light::~Light()
{
}
