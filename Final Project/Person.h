#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "BaseObject.h"
#include "Terrain.h"

class Person : public BaseObject
{

private:
	const float MOVE = 0.05f;
	bool move_flag = true;
	bool fly_flag = true;

	// NOTE: using BaseObject's position now
	//glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 3.0f);	//TODO find suitable start pos (might have to have logic for this in constructor to find terrain)
	glm::vec3 view_dir = glm::vec3(0.0f, 0.0f, -1.0f);		//TODO find suitable start direction when scene is built

	glm::mat4 view_matrix;

	Terrain* land;

	//calculates height using movable flag and terrain height
	float calcY();

	// Sets the collision box for the Person
	void setCollisionBox();

public:
	Person(Shader* shader, Terrain* terrain);
	~Person();
	
	//updates the view direction
	void changeDirection(double xdiff, double ydiff);
	
	//updates the view position
	void moveFront();
	void moveBack();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//updates movable flag (allows game pausing)
	void toggleMovable();
	bool isMovable();

	//updates fly flag
	bool isFly();
	void toggleFly();


	//updates the view matrix
	void update();
	void destroy();
	bool isDestroyed(); // TODO: Find a way to not make this mandatory for Person

	// Collision
	void onCollision(BaseObject& other);

	//TEST
	glm::mat4 getViewMatrix();

};

