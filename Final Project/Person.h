#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "BaseObject.h"

class Person : public BaseObject
{

private:
	const float MOVE = 3.0f;
	bool move_flag = true;

	glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 900.0f);	//TODO find suitable start pos (might have to have logic for this in constructor to find terrain)
	glm::vec3 view_dir = glm::vec3(0.0f, 0.0f, -1.0f);		//TODO find suitable start direction when scene is built

	glm::mat4 view_matrix;

public:
	Person(Shader* shader);
	~Person();
	
	//updates the view direction
	void changeDirection(double xdiff, double ydiff);
	
	//updates the view position
	void moveFront();
	void moveBack();
	void moveLeft();
	void moveRight();

	//updates movable flag (allows game pausing)
	void toggleMovable();
	bool isMovable();

	//updates the view matrix
	void update();
	void destroy();

};

