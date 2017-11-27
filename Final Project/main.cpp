//<<<<<<< HEAD
#include "stdafx.h"

#include "Shader.h" // Includes all of the important libraries for OpenGL and GLM (including GLFW and GLEW)
#include <iostream>
#include <vector>
#include <string>
#include "ObjectContainer.h"
#include "DirectionalLight.h"
#include "VirtualWorldContent.h"
#include <cstdlib> // for rand() and srand() -> testing object generation
#include <ctime> // for time() -> testing object generation
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include "Person.h"
#include "objloader.hpp"
#include "SOIL.h"
using namespace std;

const GLuint WIDTH = 800;
const GLuint HEIGHT = 800;

//TODO: For now 50 x 50 chunks, but we want to make the objects smaller to reduce it to 11 x 11
//Chunk width and height:
const int CHUNK_WIDTH = 50; //x coordinate
const int CHUNK_HEIGHT = 50; //z coordinate

const int NBR_DIFF_ANIMALS = 3;

// The projection and view matrices here are just testing things, will remove when we have 
// the official camera class
glm::mat4 projection_matrix;
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
const glm::vec3 eye(0.0f, 0.0f, 3.0f);

// TEST: sun and moon colors
const Color arctic_midnight(glm::vec3(0.1f), glm::vec3(0.4f, 0.4f, 0.99f)/*glm::vec3(0.39f, 0.145f, 0.13f)*/);
const Color green_northern_light(glm::vec3(0.1f), glm::vec3(0.294f, 0.933f, 0.561f));

//flags
bool fullscreen_flag = false;
bool front_flag = false;
bool back_flag = false;
bool right_flag = false;
bool left_flag = false;
bool up_flag = false;
bool down_flag = false;

//camera
Person* camera;

//constants for minimum and maximum number of objects in each generated chunk
const int MIN_NBR_IGLOOS = 1;
const int MAX_NBR_IGLOOS = 2;
const int MIN_NBR_ANIMALS = 3;
const int MAX_NBR_ANIMALS = 8;

//We create a random device for object generation:
std::random_device rd;
std::mt19937 mt(rd());

// Skyboxes
GLuint textureID_day, textureID_night;
int timeOfDay = 2;

// of the model
//float y_rotation_angle = 0.0f, x_rotation_angle = 0.0f; 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		front_flag = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
		front_flag = false;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		back_flag = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
		back_flag = false;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		left_flag = true;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
		left_flag = false;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		right_flag = true;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		right_flag = false;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		up_flag = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		up_flag = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		down_flag = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		down_flag = false;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		camera->toggleFly();
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		camera->toggleMovable();
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwTerminate();
		exit(0);
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		if (fullscreen_flag) {
			glfwSetWindowSize(window, WIDTH, HEIGHT);
		}
		else {
			int height, width;
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowSize(window, mode->width, mode->height);

			glfwSetWindowPos(window, 0, 0);

		}

		fullscreen_flag = !fullscreen_flag;
	}

	//rotate model
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
		

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	}

	//Box mode
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		timeOfDay = 1;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		timeOfDay = 2;
	}

}

//calculates camera movements with key board
void moveCamera() {

	if (front_flag) {
		camera->moveFront();
	}

	if (back_flag) {
		camera->moveBack();
	}

	if (right_flag) {
		camera->moveRight();
	}

	if (left_flag) {
		camera->moveLeft();
	}

	if (up_flag) {
		camera->moveUp();
	}

	if (down_flag) {
		camera->moveDown();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	projection_matrix = glm::perspective(glm::radians(45.0f), ((float)width) / ((float)height), 0.1f, 100.0f);
	glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (camera->isMovable()) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//gets the amount to rotate the camera
		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);

		//updates the camera direction
		camera->changeDirection(xdiff, ydiff);

		//resets the mouse location
		glfwSetCursorPos(window, width / 2, height / 2);
	}
}

void generateChunk(ObjectContainer& obj_container, Shader &mainShader, int chunk_width, int chunk_height)
{
	std::uniform_int_distribution<int> dist_igloos(MIN_NBR_IGLOOS, MAX_NBR_IGLOOS);
	int nbr_igloos = dist_igloos(mt);
	cout << "nbr igloos: " << nbr_igloos << endl;
	std::uniform_int_distribution<int> dist_animals(MIN_NBR_ANIMALS, MAX_NBR_ANIMALS);
	int nbr_animals = dist_animals(mt);
	cout << "nbr animals: " << nbr_animals << endl;

	std::uniform_int_distribution<int> dist_x(0, chunk_width);
	std::uniform_int_distribution<int> dist_z(0, chunk_height);
	cout << "Igloos: " << endl;
	for (int i = 0; i < nbr_igloos; i++)
	{
		GLfloat x = static_cast<float>(dist_x(mt));
		GLfloat z = static_cast<float>(dist_z(mt));
		cout << "i: " << i << " x: " << x << " z: " << z << endl;
		obj_container.addObject(new Igloo(&mainShader, glm::vec3(x, 0.0f, z)));
	}

	std::uniform_int_distribution<int> dist_type(1, NBR_DIFF_ANIMALS);
	cout << "Animals: " << endl;
	for (int i = 0; i < nbr_animals; i++)
	{
		GLfloat x = static_cast<float>(dist_x(mt));
		GLfloat z = static_cast<float>(dist_z(mt));
		cout << "i: " << i << " x: " << x << " z: " << z << endl;
		int type_animal = dist_type(mt);
		cout << "type: " << type_animal << endl;
		switch (type_animal)
		{
			case 1: obj_container.addObject(new Penguin(&mainShader, glm::vec3(x, 0.0f, z))); break;
			case 2: obj_container.addObject(new PolarBear(&mainShader, glm::vec3(x, 0.0f, z))); break;
			case 3: obj_container.addObject(new Eskimo(&mainShader, glm::vec3(x, 0.0f, z)));
		}
	}
	/*
	// Create only one set of objects -> once!
	GLfloat x = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	GLfloat z = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	obj_container.addObject(new Penguin(&mainShader, glm::vec3(x, 0.0f, z)));
	GLfloat x2 = static_cast<float>(min + (max - min + 5) * rand() * fraction);
	GLfloat y2 = static_cast<float>(min + (max - min + 5) * rand() * fraction);
	obj_container.addObject(new Igloo(&mainShader, glm::vec3(x2, y2, 0.0f)));
	GLfloat x3 = static_cast<float>(min + (max - min + 10) * rand() * fraction);
	GLfloat y3 = static_cast<float>(min + (max - min + 10) * rand() * fraction);
	obj_container.addObject(new PolarBear(&mainShader, glm::vec3(x3, y3, 0.0f)));
	GLfloat x4 = static_cast<float>(min + (max - min + 20) * rand() * fraction);
	GLfloat y4 = static_cast<float>(min + (max - min + 20) * rand() * fraction);
	obj_container.addObject(new Eskimo(&mainShader, glm::vec3(x4, y4, 0.0f)));*/
}

//Modified this function to allow for different IDs to be returned. Held with being able to switch between skyboxes for day and night.
GLuint loadCubemap(vector<const GLchar*> faces, GLuint texture)
{
	glGenTextures(1, &texture);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

		SOIL_free_image_data(image); //free resources
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Arctic Adventure", nullptr, nullptr);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);

	if (window == nullptr)
	{
		cout << "Failed to create GLFW window!" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW!" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// For projection matrix aspect ratio (line directly below)
	//GLfloat screen_width_height_ratio = static_cast<GLfloat>(screen_width) / screen_height;
	glViewport(0, 0, screen_width, screen_height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Shaders
	Shader mainShader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	//mainShader.useProgram();

	Shader skyBoxShader("res/shaders/skybox_vertex.shader", "res/shaders/skybox_fragment.shader");
	//skyBoxShader.useProgram();

	// Skybox textures
	std::vector<glm::vec3> skybox_day_vertices;
	std::vector<glm::vec3> skybox_day_normals; //unused
	std::vector<glm::vec2> skybox_day_UVs; //unused

	loadOBJ("res/models/cube.obj", skybox_day_vertices, skybox_day_normals, skybox_day_UVs);

	//prepare skybox VAO
	GLuint skybox_day_VAO, skybox_day_VerticesVBO;

	glGenVertexArrays(1, &skybox_day_VAO);
	glBindVertexArray(skybox_day_VAO);

	glGenBuffers(1, &skybox_day_VerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_day_VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, skybox_day_vertices.size() * sizeof(glm::vec3), &skybox_day_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//prepare skybox cubemap
	vector<const GLchar*> faces_day;
	faces_day.push_back("res/images/DayBox/right.jpg");
	faces_day.push_back("res/images/DayBox/left.jpg");
	faces_day.push_back("res/images/DayBox/top.jpg");
	faces_day.push_back("res/images/DayBox/bottom.jpg");
	faces_day.push_back("res/images/DayBox/back.jpg");
	faces_day.push_back("res/images/DayBox/front.jpg");

	glActiveTexture(GL_TEXTURE1);
	GLuint cubemapTexture = loadCubemap(faces_day, textureID_day);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	//NIGHT VERSION//
	std::vector<glm::vec3> skybox_night_vertices;
	std::vector<glm::vec3> skybox_night_normals; //unused
	std::vector<glm::vec2> skybox_night_UVs; //unused

	loadOBJ("res/models/cube.obj", skybox_night_vertices, skybox_night_normals, skybox_night_UVs);

	//prepare skybox VAO
	GLuint skybox_night_VAO, skybox_night_VerticesVBO;

	glGenVertexArrays(1, &skybox_night_VAO);
	glBindVertexArray(skybox_night_VAO);

	glGenBuffers(1, &skybox_night_VerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_night_VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, skybox_night_vertices.size() * sizeof(glm::vec3), &skybox_night_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//prepare skybox cubemap
	vector<const GLchar*> faces_night;
	faces_night.push_back("res/images/NightBox/right.jpg");
	faces_night.push_back("res/images/NightBox/left.jpg");
	faces_night.push_back("res/images/NightBox/top.jpg");
	faces_night.push_back("res/images/NightBox/bottom.jpg");
	faces_night.push_back("res/images/NightBox/back.jpg");
	faces_night.push_back("res/images/NightBox/front.jpg");

	glActiveTexture(GL_TEXTURE2);
	GLuint cubemapTexture_night = loadCubemap(faces_night, textureID_night);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_night);


	// Object container
	ObjectContainer obj_container;
	/*
	obj_container.addObject(new Penguin(&mainShader, glm::vec3(5.0f, 0.0f, 0.0f)));
	obj_container.addObject(new Igloo(&mainShader, glm::vec3(-5.0f, 0.0f, 0.0f)));
	obj_container.addObject(new PolarBear(&mainShader, glm::vec3(-5.0f, 5.0f, 0.0f)));
	obj_container.addObject(new Eskimo(&mainShader, glm::vec3(5.0f, 5.0f, 0.0f)));*/


	projection_matrix = glm::perspective(glm::radians(45.0f), (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 100.0f);

	// DirectionalLight (test)
	Color light_color = arctic_midnight;
	mainShader.useProgram();
	DirectionalLight light(&mainShader, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), light_color);

	//creates terrain TODO implement
	Terrain terrain(0, 0, 0);

	//Camera
	camera = new Person(&mainShader, &terrain);
	obj_container.addObject(camera);

	// TEST
	float init_time = static_cast<float>(glfwGetTime());
	srand(static_cast<unsigned int>(time(0)));
	int min = -10, max = 10;
	double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

	generateChunk(obj_container, mainShader, CHUNK_WIDTH, CHUNK_HEIGHT);

	/*
	GLfloat x = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	GLfloat y = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	obj_container.addObject(new Penguin(&mainShader, glm::vec3(x, y, 0.0f)));
	*/

	/*
	// Create only one set of objects -> once!
	GLfloat x = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	GLfloat y = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	obj_container.addObject(new Penguin(&mainShader, glm::vec3(x, y, 0.0f)));
	GLfloat x2 = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	GLfloat y2 = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	obj_container.addObject(new Igloo(&mainShader, glm::vec3(x2, y2, 0.0f)));
	GLfloat x3 = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	GLfloat y3 = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	obj_container.addObject(new PolarBear(&mainShader, glm::vec3(x3, y3, 0.0f)));
	GLfloat x4 = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	GLfloat y4 = static_cast<float>(min + (max - min + 1) * rand() * fraction);
	obj_container.addObject(new Eskimo(&mainShader, glm::vec3(x4, y4, 0.0f)));
	*/
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view_matrix = glm::lookAt(eye, center, up);
		//view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, 1000.0f));
		//mainShader.setMat4("view_matrix", view_matrix);
		mainShader.setMat4("projection_matrix", projection_matrix);
		// DirectionalLight color test
		//mainShader.setVec3("light_color", glm::vec3(0.54f, 1.0f, 0.47f));

		// Draw and manipulate stuff here

		skyBoxShader.useProgram();
		glm::mat4 skybox_view = glm::mat4(glm::mat3(view_matrix)); //remove the translation data

		skyBoxShader.setMat4("view_matrix", skybox_view);
		skyBoxShader.setMat4("projection_matrix", projection_matrix);

		glUniform1i(skyBoxShader.getUniformLoc("skyboxTexture"), timeOfDay); //timeOfDay calls the texture number to use

		glDepthMask(GL_FALSE);
		if (timeOfDay == 1) {
			glBindVertexArray(skybox_day_VAO);
			glDrawArrays(GL_TRIANGLES, 0, skybox_day_vertices.size());
			glBindVertexArray(0);
			light_color = arctic_midnight;
		}
		if (timeOfDay == 2) {
			glBindVertexArray(skybox_night_VAO);
			glDrawArrays(GL_TRIANGLES, 0, skybox_night_vertices.size());
			glBindVertexArray(0);
			light_color = green_northern_light;
		}
		glDepthMask(GL_TRUE);

		mainShader.useProgram();
		light.setColor(light_color);
		moveCamera(); //updates camera
		obj_container.updateAll();

		// End draw and manipulate stuff here

		glfwSwapBuffers(window);
	}

	// Deallocate resources here (glDeleteVertexArrays and glDeleteBuffers)
	glfwTerminate();

	return EXIT_SUCCESS;
}
