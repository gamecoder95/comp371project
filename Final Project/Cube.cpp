#include "Cube.h"

Cube::Cube(Shader* shader, const glm::vec3& m)
	: Object("res/models/pacman.obj", shader)
{
	move = m;
}


Cube::~Cube()
{
}

void Cube::modState()
{
	translate(move);
	if (move.x <= 0.0f)
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		color = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}
