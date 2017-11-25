#include "stdafx.h"
#include "Person.h"
#include <iostream>
#include <string>


//--------------- Constructor / Destructor --------------------------------

Person::Person(Shader* sh, Terrain* terrain) : BaseObject(sh)
{
	view_matrix = glm::lookAt(view_pos, view_pos + view_dir, glm::vec3(0.0f, 1.0f, 0.0f));
	land = terrain;
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
			float oldz = view_pos[1];
			view_pos += view_dir * MOVE;
			view_pos[1] = oldz;
		}
		else{
			view_pos += view_dir * MOVE;
			view_pos[1] = calcY();
		}
	}
}

//should be called every time the down arrow is pressed
void Person::moveBack()
{
	if (move_flag){
		if (fly_flag){
			float oldz = view_pos[1];
			view_pos -= view_dir * MOVE;
			view_pos[1] = oldz;
		}
		else{
			view_pos -= view_dir * MOVE;
			view_pos[1] = calcY();
		}
	}
}

//should be called every time the right arrow is pressed
void Person::moveRight()
{
	if (move_flag){
		view_pos += glm::cross(view_dir, glm::vec3(0.0f, 1.0f, 0.0f)) * MOVE;
		view_pos[1] = calcY();
	}
}

//should be called every time the left arrow is pressed
void Person::moveLeft()
{
	if (move_flag){
		view_pos -= glm::cross(view_dir, glm::vec3(0.0f, 1.0f, 0.0f)) * MOVE;
		view_pos[1] = calcY();
	}
}

void Person::moveUp(){
	if (move_flag && fly_flag){
		view_pos += glm::vec3(0.0f, 1.0f, 0.0f) * MOVE;
	}
}

void Person::moveDown(){
	if (move_flag && fly_flag){
		view_pos -= glm::vec3(0.0f, 1.0f, 0.0f) * MOVE;
	}
}

//calcualtes the y position of the camera based on terrain if isFly is false or returns the current value if
//isFly is true
float Person::calcY(){
	if (fly_flag){
		return view_pos[1];
	}

	return land->getHeight(view_pos[0], view_pos[2]);
}


// -------------------- Draw functions ---------------------------------------------------

//this should be called in the main game loop, will update the view_matrix for the entire scene
void Person::update()
{
	view_matrix = glm::lookAt(view_pos, view_pos + view_dir, glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4("view_matrix", view_matrix);
}

void Person::destroy(){

}

// Temporary!!
bool Person::isDestroyed() {
	return false;
}

