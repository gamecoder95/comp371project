#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

class Cube : public Object
{
private:
	glm::vec3 move;
public:
	Cube(Shader* shader, const glm::vec3& m);
	~Cube();
	void modState();

};

#endif
