//example skeleton code
//modified from http://learnopengl.com/
/*
#include "stdafx.h"
#include <vector>

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "objloader.hpp"  //include the object loader
#include <time.h>
#include <random>
#include <cstdlib>

using namespace std;

// Default window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;
//Title of the window
const char* title = "Final Project";

const string LOCATION_OBJECTS = "objects/";

glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;

// Constant vectors
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
const glm::vec3 eye(0.0f, 0.0f, 50.0f);

//Matrices
glm::mat4 view_matrix;
glm::mat4 model_matrix;

//we declare a bunch of variables that will be useful in the coming functions
int scaleRatioObj = 0;
float rotX = 0.0675f;
float rotY = 3.14159f / 2.0f;

//skip-size for Part 3:
int skip_size;

//skip-size for the Catmull Rom Spline (Part 4 & 5):
float skip_size_catmull;

//for left mouse zoom:
double oldMouseYPosZoom = 0;
//for middle mouse tilt:
double oldMouseXPosRotate = 0;
//for right mouse pan:
double oldMouseYPosRotate = 0;
//The following boolean values allow us to know if a given mouse button is held down or not
//A button being held down means we have to check the mouse position to do some rotations and zooms.
bool leftButtonDown;
bool middleButtonDown;
bool rightButtonDown;

bool leftKeyDown;
bool rightKeyDown;
bool upKeyDown;
bool downKeyDown;

//the following boolean is set to true when the user presses backspace to ask the user to reenter a new skip-size for part 3
bool redrawMesh;

//the mesh to be displayed initially is the new one modified from the skip-size
char typeMesh = 'n';

glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//P and T are used to change the rendering mode
	if (key == GLFW_KEY_P)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	else if (key == GLFW_KEY_T)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (key == GLFW_KEY_L)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	}

	//The arrow keys are used to change the camera position on the XZ plane (according to the X and Z axis)
	if (key == GLFW_KEY_LEFT)
	{
		if (GLFW_PRESS == action)
			leftKeyDown = true;
		else if (GLFW_RELEASE == action)
			leftKeyDown = false;
	}
	else if (key == GLFW_KEY_DOWN)
	{
		if (GLFW_PRESS == action)
			downKeyDown = true;
		else if (GLFW_RELEASE == action)
			downKeyDown = false;
	}
	else if (key == GLFW_KEY_UP)
	{
		if (GLFW_PRESS == action)
			upKeyDown = true;
		else if (GLFW_RELEASE == action)
			upKeyDown = false;
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		if (GLFW_PRESS == action)
			rightKeyDown = true;
		else if (GLFW_RELEASE == action)
			rightKeyDown = false;
	}

	//Backspace is used to reset the polygon 3D mesh
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		rotX = 0;
		rotY = 3.14159f / 2.0f;
		cameraPos = glm::vec3(-534.0f, -953.0f, -428.0f);
		redrawMesh = true;
	}

	//Space is used to change from the mesh with the skip-size and the original mesh
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (typeMesh == 'n')
			typeMesh = 'o';
		else
			typeMesh = 'n';
	}
}

// Is called whenever a mouse button is pressed/released via GLFW
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//Holding the right mouse button allows us to zoom in and out of the scene
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
		{
			rightButtonDown = true;
			double x;
			double y;
			glfwGetCursorPos(window, &x, &y);
			oldMouseYPosZoom = y;
		}
		else if (GLFW_RELEASE == action)
		{
			rightButtonDown = false;
			oldMouseYPosZoom = 0;
		}
	}

	//Holding the right mouse button allows us to move the camera direction
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
		{
			leftButtonDown = true;
			double x;
			double y;
			glfwGetCursorPos(window, &x, &y);
			oldMouseXPosRotate = x;
			oldMouseYPosRotate = y;
		}
		else if (GLFW_RELEASE == action)
		{
			leftButtonDown = false;
			oldMouseXPosRotate = 0;
			oldMouseYPosRotate = 0;
		}
	}
}
//This function is used to zoom in and out of the scene when the right mouse button is held down
void zoom(GLFWwindow* window)
{
	if (rightButtonDown)
	{
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		double difference = y - oldMouseYPosZoom;
		cameraPos.y -= float((difference));
		oldMouseYPosZoom = y;
	}
}
//This function is used to move the camera direction when the left mouse button is held down
void moveCameraDirection(GLFWwindow* window)
{
	if (leftButtonDown)
	{
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		double difference_x = x - oldMouseXPosRotate;
		rotX += float((difference_x / 400.f));
		oldMouseXPosRotate = x;
		double difference_y = y - oldMouseYPosRotate;
		rotY += float((difference_y / 400.f));
		oldMouseYPosRotate = y;
	}
}
void moveCameraPosition()
{
	if (leftKeyDown)
		cameraPos.x += 2.f;
	if (rightKeyDown)
		cameraPos.x -= 2.f;
	if (upKeyDown)
		cameraPos.z += 2.f;
	if (downKeyDown)
		cameraPos.z -= 2.f;
}
//This method generates an array of indices used for the EBOs
vector<GLuint> generateIndiceArray(vector<glm::vec3> mesh, int imgWidth, int imgHeight)
{
	vector<GLuint> indices;
	indices.reserve(mesh.size());
	for (int i = 0; i < mesh.size() - imgWidth; i++)
	{
		if (i%imgHeight < imgWidth - 1)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + imgWidth);
			indices.push_back(i + 1);
			indices.push_back(i + imgWidth);
			indices.push_back(i + imgWidth + 1);
		}
	}
	return indices;
}


//Function to initialize the vertex shader
GLuint initializeVertexShader()
{
	// Read the Vertex Shader code from the file
	string vertex_shader_path = "vertex.shader";
	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	//vertexShader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return vertexShader;
}
//Function to initialize the fragment shader
GLuint initializeFragmentShader()
{
	// Read the Fragment Shader code from the file
	string fragment_shader_path = "fragment.shader";
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return fragmentShader;
}

//Function to link both shaders
GLuint linkShaders(GLuint vertexShader, GLuint fragmentShader)
{
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); //free up memory
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

static void loadOBJinVAO(string file_name, std::vector<glm::vec3>* vertices, GLuint* VAO)
{
	
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	
	string relative_path = LOCATION_OBJECTS + file_name;

	loadOBJ(relative_path.c_str(), *vertices, normals, UVs); //read the vertices from the teapot.obj file

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s)pacman_VAO and attribute pointer(s).
	GLuint VBO, normals_VBO;

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(glm::vec3), &(vertices->front()), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	projection_matrix = glm::perspective(45.0f, (GLfloat)frameBufferWidth / (GLfloat)frameBufferHeight, 0.0f, 100.0f);

	// Build and compile our shader program
	GLuint vertexShader = initializeVertexShader();
	GLuint fragmentShader = initializeFragmentShader();
	GLuint shaderProgram = linkShaders(vertexShader, fragmentShader);

	glUseProgram(shaderProgram);

	std::vector<glm::vec3> penguin1_vertices;
	GLuint penguin1_VAO;
	loadOBJinVAO("penguin.obj", &penguin1_vertices, &penguin1_VAO);

	std::vector<glm::vec3> polar_bear_vertices;
	GLuint polar_bear_VAO;
	loadOBJinVAO("polarbear.obj", &polar_bear_vertices, &polar_bear_VAO);

	std::vector<glm::vec3> eskimo_vertices;
	GLuint eskimo_VAO;
	loadOBJinVAO("nana.obj", &eskimo_vertices, &eskimo_VAO);

	std::vector<glm::vec3> igloo_vertices;
	GLuint igloo_VAO;
	loadOBJinVAO("igloo_mod.obj", &igloo_vertices, &igloo_VAO);

	triangle_scale = glm::vec3(1.0f);

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "object_type");

	view_matrix = glm::lookAt(eye, center, up);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


	glfwWindowHint(GL_DEPTH_BITS, 24);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthRange(0.1f, 100.0f);
	glDepthMask(GL_TRUE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//The following 3 functions are called to zoom in and out of the scene, tilt and pan the camera. (all of those actions are done by mouse input)
		zoom(window);
		moveCameraPosition();
		moveCameraDirection(window);

		model_matrix = glm::scale(model_matrix, triangle_scale);

		view_matrix = glm::lookAt(eye, center, up);
		view_matrix = glm::rotate(view_matrix, rotX, glm::vec3(0.0f, 1.0f, 0.0f));
		view_matrix = glm::rotate(view_matrix, rotY, glm::vec3(1.0f, 0.0f, 0.0f));
		view_matrix = glm::translate(view_matrix, cameraPos);


		//The following is used to update the framebuffer when the size of the window is changed to maintain the correct aspect ratio
		//we also apply the desired cameraZoom to the projection matrix
		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
		glViewport(0, 0, frameBufferWidth, frameBufferHeight);
		projection_matrix = glm::perspective(45.0f, (GLfloat)frameBufferWidth / (GLfloat)frameBufferHeight, 0.1f, 1000.0f);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		
		//PROJECT: DRAWING
		glUniform1i(colorLoc, 0);

		glBindVertexArray(penguin1_VAO);
		glDrawArrays(GL_TRIANGLES, 0, penguin1_vertices.size());
		glBindVertexArray(0);

		glUniform1i(colorLoc, 1);
		model_matrix = glm::translate(model_matrix, glm::vec3(10.0f, 0.0f, 0.0f));
		model_matrix = glm::scale(model_matrix, glm::vec3(0.032f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glBindVertexArray(polar_bear_VAO);
		glDrawArrays(GL_TRIANGLES, 0, polar_bear_vertices.size());
		glBindVertexArray(0);

		model_matrix = glm::mat3(1.0f);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

		glUniform1i(colorLoc, 2);
		model_matrix = glm::translate(model_matrix, glm::vec3(5.0f, 0.0f, 0.0f));
		model_matrix = glm::scale(model_matrix, glm::vec3(0.2f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glBindVertexArray(eskimo_VAO);
		glDrawArrays(GL_TRIANGLES, 0, eskimo_vertices.size());
		glBindVertexArray(0);

		model_matrix = glm::mat3(1.0f);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		
		glUniform1i(colorLoc, 1);
		model_matrix = glm::translate(model_matrix, glm::vec3(17.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glBindVertexArray(igloo_VAO);
		glDrawArrays(GL_TRIANGLES, 0, igloo_vertices.size());
		glBindVertexArray(0);

		model_matrix = glm::mat3(1.0f);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
*/