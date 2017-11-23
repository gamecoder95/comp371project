#include "stdafx.h"
#include "Person.h"


//--------------- Constructor / Destructor --------------------------------

Person::Person(Shader* sh) : BaseObject(sh)
{
	view_matrix = glm::lookAt(view_pos, view_pos + view_dir, glm::vec3(0.0f, 1.0f, 0.0f));
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

//---------------- Direction changing function ---------------------------

void Person::changeDirection(double xdiff, double ydiff)
{
	/* This logic must be added to the mousePositionCallback function (this snippet assumes the Person variable is named camera)
	if(camera.isMovable()){
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//gets the amount to rotate the camera
		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);

		//updates the camera direction
		camera.changeDirection(xdiff, ydiff)

		//resets the mouse location
		glfwSetCursorPos(window, width / 2, height / 2);
	}
	*/

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
		view_pos += view_dir * MOVE;
	}
}

//should be called every time the down arrow is pressed
void Person::moveBack()
{
	if (move_flag){
		view_pos -= view_dir * MOVE;
	}
}

//should be called every time the right arrow is pressed
void Person::moveRight()
{
	if (move_flag){
		view_pos += glm::cross(view_dir, glm::vec3(0, 1, 0)) * MOVE;
	}
}

//should be called every time the left arrow is pressed
void Person::moveLeft()
{
	if (move_flag){
		view_pos -= glm::cross(view_dir, glm::vec3(0, 1, 0)) * MOVE;
	}
}


// -------------------- Draw functions ---------------------------------------------------

//this should be called in the main game loop, will updates the view_matrix for the entire scene
void Person::update()
{
	view_matrix = glm::lookAt(view_pos, view_pos + view_dir, glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4("view_matrix", view_matrix);
}

void Person::destroy(){

}

