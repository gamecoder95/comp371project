#include "stdafx.h"
#include "Person.h"
#include <iostream>
#include <string>
#include "VirtualWorldContent.h"


//--------------- Constructor / Destructor --------------------------------

Person::Person(Shader* sh, Terrain* terrain) : BaseObject(sh)
{
	view_matrix = glm::lookAt(position, position + view_dir, glm::vec3(0.0f, 1.0f, 0.0f));
	land = terrain;
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	setCollisionBox();
}


Person::~Person()
{
	destroy();
}


// --------------- Movability tests -------------------------------------

void Person::toggleMovable()
{
	move_flag = !move_flag;
}

bool Person::isMovable()
{
	return move_flag;
}

// ----------------- Fly Flag --------------------------------------------

void Person::toggleFly(){
	fly_flag = !fly_flag;
}

bool Person::isFly(){
	return fly_flag;
}

//---------------- Direction changing function ---------------------------

void Person::changeDirection(double xdiff, double ydiff)
{

	//makes sure he user is pressing a button to move look around
	if (move_flag){
		//makes user not to reck the view matrix (multiplying by 0 isn't good)
		if (xdiff != 0 && ydiff != 0){
			//places the movement 
			glm::vec4 mod_v4 = glm::vec4(xdiff*0.001, ydiff*-0.001, 0, 1);
			view_dir = glm::normalize(view_dir + glm::vec3(mod_v4 * view_matrix));
		}
	}
}

// ------------------ Camera position chaning functions ----------------------------

//should be called every time the up arrow key is pressed
void Person::moveFront()
{
	if (move_flag){
		if (fly_flag){
			float oldz = position[1];
			position += view_dir * MOVE;
			position[1] = oldz;
		}
		else{
			position += view_dir * MOVE;
			position[1] = calcY();
		}
	}
}

//should be called every time the down arrow is pressed
void Person::moveBack()
{
	if (move_flag){
		if (fly_flag){
			float oldz = position[1];
			position -= view_dir * MOVE;
			position[1] = oldz;
		}
		else{
			position -= view_dir * MOVE;
			position[1] = calcY();
		}
	}
}

//should be called every time the right arrow is pressed
void Person::moveRight()
{
	if (move_flag){
		position += glm::cross(view_dir, glm::vec3(0.0f, 1.0f, 0.0f)) * MOVE;
		position[1] = calcY();
	}
}

//should be called every time the left arrow is pressed
void Person::moveLeft()
{
	if (move_flag){
		position -= glm::cross(view_dir, glm::vec3(0.0f, 1.0f, 0.0f)) * MOVE;
		position[1] = calcY();
	}
}

void Person::moveUp(){
	if (move_flag && fly_flag){
		position += glm::vec3(0.0f, 1.0f, 0.0f) * MOVE;
	}
}

void Person::moveDown(){
	if (move_flag && fly_flag){
		position -= glm::vec3(0.0f, 1.0f, 0.0f) * MOVE;
	}
}

//calcualtes the y position of the camera based on terrain if isFly is false or returns the current value if
//isFly is true
float Person::calcY(){
	if (fly_flag){
		return position[1];
	}

	return land->getHeightAt(position[0], position[2]);
}

// -------------------- Collision Functions ----------------------------------------------

// For now, set the collision box to be merely a unit volume cube
// TODO: adjust the y-xis values so that maybe it works better with climbing up hills
void Person::setCollisionBox()
{
	float collBoxVal = 1.0f;
	// z-axis
	collisionBox.back = -collBoxVal;
	collisionBox.front = collBoxVal;

	// x-axis
	collisionBox.left = -collBoxVal;
	collisionBox.right = collBoxVal;

	// y-axis
	collisionBox.bottom = -collBoxVal;
	collisionBox.top = collBoxVal;
}

// When colliding with another object, prevent the person from going through
// This is done by checking the collisions on each axis, and then offsetting the player
// by the correct amount backwards
void Person::onCollision(BaseObject& other)
{
	cout << "HIT" << endl;
	dynamic_cast<Object&>(other).destroy();

	//// x-collision
	//if (getLeft() < other.getRight())
	//{
	//	position.x += (other.getRight() - getLeft());
	//}
	//else if (getRight() > other.getLeft())
	//{
	//	position.x += (other.getLeft() - getRight());
	//}

	//// z-collision
	//if (getFront() > other.getBack())
	//{
	//	position.z += (other.getBack() - getFront());
	//}
	//else if (getBack() < other.getFront())
	//{
	//	position.z += (other.getFront() - getBack());
	//}

	//// y-collision
	//if (getBottom() < other.getTop())
	//{
	//	position.y += (other.getTop() - getBottom());
	//}
	//else if (getTop() > other.getBottom())
	//{
	//	position.y += (other.getBottom() - getTop());
	//}
}


// -------------------- Draw functions ---------------------------------------------------

//this should be called in the main game loop, will update the view_matrix for the entire scene
void Person::update()
{
	view_matrix = glm::lookAt(position, position + view_dir, glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4("view_matrix", view_matrix);
	shader->setVec3("view_pos", position);
}

void Person::destroy(){

}

// Temporary!!
bool Person::isDestroyed() {
	return false;
}

